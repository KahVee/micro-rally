#include "HostService.hpp"

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

void HostService::StartGame()
{
    gameRunning_ = true;
    gameInit_ = true;
}

void HostService::StopGame()
{
    gameRunning_ = false;
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
            sf::TcpSocket* client = new sf::TcpSocket;
            if(listener_.accept(*client) == sf::Socket::Done)
            {
                // TODO CHECK MAX CLIENTS
                // TODO IF CLIENTS MORE THAN 100
                // Find unused id
                sf::Int32 id = 0;
                for(sf::Int32 i = 0; i < 100; i++)
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
                        id = i;
                        break;
                    }
                }
                // Add new client
                clients_.push_back({id, client, ""});
                selector_.add(*client);
            }
            else
            {
                delete client;
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
                        // Receive data on socket
                        sf::Packet packet;
                        status = ReceiveIfReady(*client.socket, packet);//client.socket->receive(packet);
                        if(status == sf::Socket::Done)
                        {
                            // Received data in packet
                            std::string messageType;
                            if(packet >> messageType)
                            {
                                if(messageType == "CHAT_MESSAGE")
                                {
                                    // Handle chat message
                                    std::string clientName;
                                    std::string chatMessage;
                                    packet >> clientName >> chatMessage;
                                    sf::Packet sendPacket;
                                    sendPacket << "CHAT_MESSAGE" << clientName << chatMessage;
                                    // Send to all chatters
                                    SendToAll(sendPacket);
                                }
                                else if (messageType == "PING")
                                {
                                    // Handle ping
                                    sf::Packet sendPacket;
                                    sendPacket << "PING";
                                    SendToOne(client.socket, sendPacket);
                                }
                                else if (messageType == "CLIENT_CONNECT")
                                {
                                    // Handle client connection message
                                    std::string clientName;
                                    packet >> clientName;
                                    client.name = clientName;
                                    // Send client id to client
                                    sf::Packet idPacket;
                                    idPacket << "CLIENT_ID" << client.id;
                                    SendToOne(client.socket, idPacket);
                                    // Send updated list of clients to clients
                                    sf::Int32 numberOfClients = clients_.size();
                                    sf::Packet sendPacket;
                                    sendPacket << "CLIENT_CONNECT" << numberOfClients;
                                    for(auto client2 : clients_)
                                    {
                                        sendPacket << client2.name << client2.id;
                                    }
                                    // Send to all chatters
                                    SendToAll(sendPacket);
                                }
                                else if (messageType == "PLAYER_INFO")
                                {
                                    // Update player info
                                    packet >> client.id >> client.transform >> client.velocity >> client.angularVelocity;
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
        // Send game packets here
        if(gameInit_)
        {
            // Send info of game starting to clients
            sf::Packet sendPacket;
            sendPacket << "GAME_START";
            SendToAll( sendPacket);

            sf::Int32 numberOfClients = clients_.size();
            sf::Packet sendPacket2;
            sendPacket2 << "CLIENT_CONNECT" << numberOfClients;
            for(auto client : clients_)
            {
                sendPacket2 << client.name << client.id;
            }
            // Send to all chatters
            SendToAll(sendPacket2);
            gameInit_ = false;
        }
        // Game loop here
        sf::Int32 numberOfClients = clients_.size();
        sf::Packet sendPacket;
        sendPacket << "PLAYER_INFO" << numberOfClients;
        for(auto& client : clients_)
        {
            sendPacket << client.id << client.transform << client.velocity << client.angularVelocity;
        }
        SendToAll(sendPacket);
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
            sendPacket << "CLIENT_DISCONNECT" << client.name << client.id;
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