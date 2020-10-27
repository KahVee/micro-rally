#include "HostService.hpp"

HostService::HostService()
{
    running_ = false;
}

HostService::~HostService()
{
    for(auto client : clients_)
    {
        delete client;
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
                    clients_.push_back(client);
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
                for(auto client : clients_)
                {
                    if(selector_.isReady(*client))
                    {
                        sf::Packet packet;
                        if(client->receive(packet) == sf::Socket::Done)
                        {
                            // Received data in packet
                            std::string messageType;
                            if(packet >> messageType)
                            {
                                if(messageType == "CHAT_MESSAGE")
                                {
                                    // Handle chat message
                                    std::string playerName;
                                    std::string chatMessage;
                                    packet >> playerName >> chatMessage;
                                    sf::Packet sendPacket;
                                    sendPacket << "CHAT_MESSAGE" << playerName << chatMessage;
                                    // Send to all chatters
                                    SendToAll(sendPacket);
                                }
                                else if (messageType == "PING")
                                {
                                    // Handle ping
                                    sf::Packet sendPacket;
                                    sendPacket << "PING";
                                    SendToOne(client, sendPacket);
                                }
                            }
                            // Nothing implemented for failure to read    
                        }
                    }
                }
                // Delete null pointers in case disconnected clients were deleted
                clients_.erase(std::remove(clients_.begin(), clients_.end(), nullptr), clients_.end());
            }
        }
    }
    // Send end message to all clients
    sf::Packet packet;
    packet << "HOST_QUIT";
    SendToAll(packet);
    // Reset hostservice
    for(auto client : clients_)
    {
        client->disconnect();
    }
    clients_.erase(std::remove(clients_.begin(), clients_.end(), nullptr), clients_.end());
    for(auto client : clients_)
    {
        delete client;
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
    for(auto client : clients_)
    {
        SendToOne(client, packet);
    }
}

void HostService::SendToOne(sf::TcpSocket* client, sf::Packet& packet)
{
    // Check if client connected
    if(client->getLocalPort())
    {
        client->send(packet);
    }
    else
    {
        // If not connected remove from connections
        selector_.remove(*client);
        delete client;
    }
}