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
        connectPacket << CLIENT_CONNECT << playerName;
        socket_.send(connectPacket);
    }
    else
    {
        socket_.disconnect();
    }
    return status;
}

void ClientService::Disconnect()
{
    // Reset clientservice
    selector_.clear();
    socket_.disconnect();
    id_ = -1;
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
        sf::Packet packetCopy(packet);
        NetworkMessageType messageType;
        packet >> messageType;
        if(messageType == PING)
        {
            pingClock_.restart();
        }
        socket_.send(packetCopy);
    }
    else
    {
        // If not connected run Disconnect-method
        Disconnect();
    }
}

void ClientService::Receive()
{
    // Loop through all data on socket
    sf::Socket::Status status = sf::Socket::Done;
    while (status == sf::Socket::Done)
    {
        // Received data in packet
        sf::Packet packet;
        // Receive data if ready
        status = ReceiveIfReady(packet);
        if(status == sf::Socket::Done)
        {
            sf::Packet packetCopy(packet);
            NetworkMessageType messageType;
            if(packet >> messageType)
            {
                // Handle different message types
                if(messageType == PING)
                {
                    sf::Time ping = pingClock_.getElapsedTime();
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(4) << ping.asSeconds();
                    ss << " seconds";
                    packetCopy << ss.str();
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_ID)
                {
                    packet >> id_;
                }
                else if (messageType == GAME_START)
                {
                    sceneManager_->ChangeScene("game");
                }
                else if (messageType == GAME_FINISH)
                {
                    sceneManager_->ChangeScene("scorescreen");
                }
                else if (messageType == CHAT_MESSAGE)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_CONNECT)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_DISCONNECT)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_DATA)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_WIN)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
                else if (messageType == CLIENT_RANK)
                {
                    sceneManager_->HandlePacket(packetCopy);
                }
            }        
        }
        else if(status == sf::Socket::Disconnected)
        {
            Disconnect();
        }
    }
}

sf::Int32 ClientService::GetId() const
{
    return id_;
}

sf::Socket::Status ClientService::ReceiveIfReady(sf::Packet& packet)
{   
    if (selector_.wait(sf::microseconds(1)))
        return socket_.receive(packet);
    else
        return sf::Socket::NotReady;
}