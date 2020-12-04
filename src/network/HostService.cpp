#include "HostService.hpp"

HostService::HostService()
{
    running_ = false;
}

HostService::~HostService()
{
    for(auto& client : clients_)
    {
        delete client.socket;
    }
}

sf::Socket::Status HostService::Listen(unsigned short port)
{
    sf::Socket::Status status = listener_.listen(port);
    if(status == sf::Socket::Done)
    {
        selector_.add(listener_);
    }
    return status;
}

void HostService::Start()
{
    running_ = true;
    while(running_)
    {
        Receive();
        RunGame();
    }
    // Reset hostservice
    for(auto& client : clients_)
    {
        client.socket->disconnect();
        delete client.socket;
    }
    clients_.clear();
    selector_.clear();
    listener_.close();
}

void HostService::Stop()
{
    running_ = false;
}

bool HostService::IsRunning()
{
    return running_;
}

bool HostService::IsGameRunning()
{
    return gameRunning_;
}

void HostService::SendToAll(sf::Packet& packet)
{
    for(auto& client : clients_)
    {
        SendToOne(client.socket, packet);
    }
}

void HostService::SendToOne(sf::TcpSocket*& socket, sf::Packet& packet)
{
    // Check if client connected
    if(socket->getLocalPort())
    {
        socket->send(packet);
    }
}

void HostService::Receive()
{
    // Wait for data on any socket
    if(selector_.wait(sf::seconds(0.05f)))
    {
        // Test the listener
        if(selector_.isReady(listener_))
        {
            sf::TcpSocket* socket = new sf::TcpSocket;
            if(listener_.accept(*socket) == sf::Socket::Done)
            {
                if(gameRunning_ || clients_.size() >= MAX_CLIENTS)
                {
                    socket->disconnect();
                    delete socket;
                }
                else
                {
                    // Add new client
                    clients_.push_back({GetUnusedId(), socket, ""});
                    selector_.add(*socket);   
                }
            }
            else
            {
                delete socket;
            }
        }
        else
        {
            // Test other sockets
            for(auto& client : clients_)
            {
                if(selector_.isReady(*client.socket))
                {
                    // Loop through all data on socket
                    sf::Socket::Status status = sf::Socket::Done;
                    while (status == sf::Socket::Done)
                    {
                        // Received data in packet
                        sf::Packet packet;
                        // Receive data on socket
                        status = ReceiveIfReady(*client.socket, packet);
                        if(status == sf::Socket::Done)
                        {
                            sf::Packet packetCopy(packet);
                            NetworkMessageType messageType;
                            if(packet >> messageType)
                            {
                                // Handle different message types
                                if(messageType == CHAT_MESSAGE)
                                {
                                    // Handle chat message
                                    SendToAll(packetCopy);
                                }
                                else if (messageType == PING)
                                {
                                    // Handle ping
                                    SendToOne(client.socket, packetCopy);
                                }
                                else if (messageType == CLIENT_CONNECT)
                                {
                                    // Handle client connection message
                                    packet >> client.name;
                                    // Send client id to client
                                    sf::Packet idPacket;
                                    idPacket << CLIENT_ID << client.id;
                                    SendToOne(client.socket, idPacket);
                                    // Send other clients info of new client
                                    packetCopy << client.id;
                                    SendToAll(packetCopy);
                                    // Send new client info of other clients
                                    for(auto& client2 : clients_)
                                    {
                                        sf::Packet sendPacket;
                                        sendPacket << CLIENT_CONNECT << client2.name << client2.id;
                                        SendToOne(client.socket, sendPacket);
                                    }
                                }
                                else if (messageType == CLIENT_DATA)
                                {
                                    // Update player info
                                    packet >> client.id >> client.transform >> client.velocity >> client.angularVelocity >> client.steeringAngle;
                                }
                                else if (messageType == GAME_START && client.id == 0)
                                {
                                    // Send game init packets here
                                    gameRunning_ = true;
                                    // Send info of game starting to clients
                                    SendToAll(packetCopy);
                                    // Inform all clients of other players
                                    for(auto& client2 : clients_)
                                    {
                                        sf::Packet sendPacket2;
                                        sendPacket2 << CLIENT_START << client2.name << client2.id << client2.car;
                                        SendToAll(sendPacket2);
                                    }
                                }
                                else if (messageType == CLIENT_WIN)
                                {
                                    packetCopy << client.id;
                                    SendToAll(packetCopy);
                                    client.finished = true;
                                    lastFinishRanking_ += 1;
                                    client.ranking = lastFinishRanking_;
                                }
                                else if (messageType == CLIENT_CAR)
                                {
                                    // Read client car type into memory
                                    packet >> client.car;
                                }
                                else if (messageType == OBJECT_CREATE)
                                {
                                    sf::Int32 id = -1;
                                    NetworkDynamicObject object;
                                    packet >> id >> object.transform >> object.velocity >> object.angularVelocity;
                                    networkObjects_[id] = object;
                                }
                                else if (messageType == OBJECT_DATA)
                                {
                                    sf::Int32 id = -1;
                                    packet >> id;
                                    packet >> networkObjects_[id].transform >> networkObjects_[id].velocity >> networkObjects_[id].angularVelocity;
                                }
                            }
                        }
                        else if (status == sf::Socket::Disconnected)
                        {
                            // If not connected then disconnect the socket and remove from selector
                            // THE ORDER OF THE FOLLOWING METHODS IS VERY IMPORTANT
                            selector_.remove(*client.socket);
                            client.socket->disconnect();
                        }
                    }
                }
            }
            // Handle disconnected clients
            HandleDisconnectedClients();
        }
    }
}

void HostService::RunGame()
{
    if(gameRunning_)
    {
        // Game loop here
        // Send info of other clients to clients
        for(auto& client : clients_)
        {
            if(!client.finished)
            {
                // Send client data
                sf::Packet sendPacket;
                sendPacket << CLIENT_DATA << client.id << client.transform << client.velocity << client.angularVelocity << client.steeringAngle;
                SendToAll(sendPacket);
            }
        }
        // Send object data
        for(auto& object : networkObjects_)
        {
            sf::Packet sendPacket;
            sendPacket << OBJECT_DATA << object.first << object.second.transform << object.second.velocity << object.second.angularVelocity;
            SendToAll(sendPacket);
        }
        // Check if everyone has finished to finish game
        bool allFinished = true;
        for(auto& client2 : clients_)
        {
            if(!client2.finished)
            {
                allFinished = false;
                break;
            }
        }
        // If all finished finish game and send finish game message to all
        if (allFinished)
        {
            // Reset game data 
            lastFinishRanking_ = 0;
            gameRunning_ = false;
            networkObjects_.clear();
            sf::Packet sendPacket;
            sendPacket << GAME_FINISH;
            SendToAll(sendPacket);
            for(auto& client2: clients_)
            {
                sf::Packet sendPacket2;
                sendPacket2 << CLIENT_RANK << client2.id << client2.name << client2.ranking;
                SendToAll(sendPacket2);
            }
        }
    }
}

void HostService::HandleDisconnectedClients()
{
    // Inform clients of disconnected clients
    for(auto& client : clients_)
    {
        if(!client.socket->getLocalPort())
        {
            sf::Packet sendPacket;
            sendPacket << CLIENT_DISCONNECT << client.name << client.id;
            // Send to all chatters
            SendToAll(sendPacket);
        }
    }
    // Remove disconnected clients and delete memory
    for(auto& client : clients_)
    {
        if(!client.socket->getLocalPort())
        {
            delete client.socket;
            client.socket = nullptr;
        }
    }
    // Delete null pointers from deleted clients from list
    clients_.remove_if([](const Client& client){return client.socket == nullptr;});
}

sf::Socket::Status HostService::ReceiveIfReady(sf::TcpSocket& socket, sf::Packet& packet)
{
    sf::SocketSelector selector;
    selector.add(socket);
    if (selector.wait(sf::microseconds(1)))
        return socket.receive(packet);
    else
        return sf::Socket::NotReady;
}

sf::Int32 HostService::GetUnusedId()
{
    // Find unused id
    for(sf::Int32 i = 0; i < MAX_CLIENTS; i++)
    {
        bool idUsed = false;
        for(auto& client : clients_)
        {
            if(client.id == i)
            {
                idUsed = true;
                break;
            }
        }
        if(!idUsed)
        {
            return i;
        }
    }
    return -1;
}