#include "ClientService.hpp"

void ClientService::Init(SceneManager* sceneManager)
{
    sceneManager_ = sceneManager;
}

sf::Socket::Status ClientService::Connect(const sf::IpAddress &address, unsigned short port, sf::Time timeout, const std::string& playerName)
{
    sf::Socket::Status status = socket_.connect(address, port, timeout);
    if(status == sf::Socket::Done)
    {
        selector_.add(socket_);
        // Send client info to host
        sf::Packet connectPacket;
        connectPacket << "CLIENT_CONNECT" << playerName;
        socket_.send(connectPacket);
    }
    return status;
}

void ClientService::Disconnect()
{
    // Reset clientservice
    selector_.clear();
    socket_.disconnect();
    sceneManager_->ChangeScene("mainMenu");
}

bool ClientService::IsConnected()
{
    return socket_.getLocalPort();
}

void ClientService::Send(sf::Packet& packet)
{
    // Check if client connected
    if(socket_.getLocalPort())
    {
        std::string messageType;
        packet >> messageType;
        if(messageType == "PING")
        {
            pingClock_.restart();
            sf::Packet pingPacket;
            pingPacket << "PING";
            socket_.send(pingPacket);
        }
        else if(messageType == "CHAT_MESSAGE")
        {
            std::string playerName;
            std::string message;
            packet >> playerName >> message;
            sf::Packet messagePacket;
            messagePacket << "CHAT_MESSAGE" << playerName << message;
            socket_.send(messagePacket);
        }
    }
    else
    {
        // If not connected reset and set running_ = false
        Disconnect();
    }
}

void ClientService::Receive()
{
    // Wait for data on socket
    sf::Packet packet;
    sf::Socket::Status status = ReceiveWithTimeout(packet, sf::microseconds(1));
    if(status == sf::Socket::Done)
    {
        // Received data in packet
        std::string messageType;
        if(packet >> messageType)
        {
            if(messageType == "PING")
            {
                sf::Time ping = pingClock_.getElapsedTime();
                std::stringstream ss;
                ss << std::fixed << std::setprecision(4) << ping.asSeconds();
                ss << " seconds";
                sf::Packet sendPacket;
                sendPacket << "PING" << ss.str();
                sceneManager_->HandlePacket(sendPacket);
            }
            else if (messageType == "CHAT_MESSAGE")
            {
                std::string playerName;
                std::string message;
                packet >> playerName >> message;
                sf::Packet sendPacket;
                sendPacket << "CHAT_MESSAGE" << playerName << message;
                sceneManager_->HandlePacket(sendPacket);
            }
            else if (messageType == "CLIENT_CONNECT")
            {
                sf::Int32 numberOfClients;
                packet >> numberOfClients;
                for(int i = 0; i < numberOfClients; i++)
                {
                    std::string clientName;
                    sf::Int32 id;
                    packet >> clientName >> id;
                    sf::Packet sendPacket;
                    sendPacket << "CLIENT_CONNECT" << clientName << id;
                    sceneManager_->HandlePacket(sendPacket);      
                }
            }
            
            else if (messageType == "CLIENT_DISCONNECT")
            {
                std::string clientName;
                sf::Int32 id;
                packet >> clientName >> id;
                sf::Packet sendPacket;
                sendPacket << "CLIENT_DISCONNECT" << clientName << id;
                sceneManager_->HandlePacket(sendPacket);
            }
        }        
    }
    else if(status == sf::Socket::Disconnected)
    {
        Disconnect();
    }
}

sf::Socket::Status ClientService::ReceiveWithTimeout(sf::Packet& packet, sf::Time timeout)
{   
    if (selector_.wait(timeout))
        return socket_.receive(packet);
    else
        return sf::Socket::NotReady;
}