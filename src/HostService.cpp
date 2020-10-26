#include "HostService.hpp"

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
                                    // Do something
                                    std::string chatMessage;
                                    packet >> chatMessage;
                                    sf::Packet sendPacket;
                                    sendPacket << "CHAT_MESSAGE" << chatMessage;
                                    // Send to all chatters
                                    for(auto sendClient : clients_)
                                    {
                                        sendClient->send(sendPacket);
                                    }
                                }
                                else if (messageType == "PING")
                                {
                                    // Handle ping
                                    sf::Packet sendPacket;
                                    sendPacket << "PING";
                                    client->send(sendPacket);
                                }
                            }
                            // Nothing implemented for failure to read    
                        }
                    }
                }
            }
        }
    }
}

void HostService::Stop()
{
    running_ = false;
}

bool HostService::IsRunning()
{
    return running_;
}