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
        // Wait for data on any socket
        if(selector_.wait(sf::seconds(0.05f)))
        {
            // Test the listener
            if(selector_.isReady(listener_))
            {
                sf::TcpSocket* client = new sf::TcpSocket;
                if(listener_.accept(*client) == sf::Socket::Done)
                {
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
                // TODO HANDLE DISCONNECTED SOCKETS IN A BETTER WAY
                // Test other sockets
                for(auto& client : clients_)
                {
                    if(selector_.isReady(*client.socket))
                    {
                        sf::Packet packet;
                        sf::Socket::Status status = client.socket->receive(packet);
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
                            }
                        }
                        else if (status == sf::Socket::Disconnected)
                        {
                            // If not connected remove from connections
                            RemoveClient(client.socket);
                        }
                    }
                }
                // TODO inform clients of disconnected client
                for(auto& client : clients_)
                {
                    if(client.socket == nullptr)
                    {
                        sf::Packet sendPacket;
                        sendPacket << "CLIENT_DISCONNECT" << client.name << client.id;
                        // Send to all chatters
                        for(auto& client2 : clients_)
                        {
                            if(client2.socket != nullptr)
                            {
                                SendToOne(client2.socket, sendPacket);
                            }
                        }
                    }
                }
                // Delete null pointers in case disconnected clients were deleted
                clients_.remove_if([](const Client& client){return client.socket == nullptr;});
            }
        }
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

void HostService::SendToAll(sf::Packet& packet)
{
    for(auto& client : clients_)
    {
        SendToOne(client.socket, packet);
    }
}

void HostService::SendToOne(sf::TcpSocket*& client, sf::Packet& packet)
{
    // Check if client connected
    if(client->getLocalPort())
    {
        client->send(packet);
    }
    else
    {
        // If not connected remove from connections
        RemoveClient(client);
    }
}

void HostService::RemoveClient(sf::TcpSocket*& client)
{
    selector_.remove(*client);
    client->disconnect();
    delete client;
    client = nullptr;
}