#include "ClientService.hpp"

void ClientService::Init(SceneManager* sceneManager)
{
    sceneManager_ = sceneManager;
}

sf::Socket::Status ClientService::Connect(const sf::IpAddress &address, unsigned short port, sf::Time timeout)
{
    sf::Socket::Status status = socket_.connect(address, port, timeout);
    if(status == sf::Socket::Done)
    {
        selector_.add(socket_);
    }
    return status;
}

void ClientService::Disconnect()
{
    // Reset clientservice
    selector_.clear();
    socket_.disconnect();
    sf::Packet packet;
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
                sf::Packet pingPacket;
                pingPacket << "PING" << ss.str();
                sceneManager_->HandlePacket(pingPacket);
            }
            else if (messageType == "CHAT_MESSAGE")
            {
                std::string playerName;
                std::string message;
                packet >> playerName >> message;
                sf::Packet messagePacket;
                messagePacket << "CHAT_MESSAGE" << playerName << message;
                sceneManager_->HandlePacket(messagePacket);
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