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
        NetworkMessageType messageType;
        packet >> messageType;
        if(messageType == PING)
        {
            pingClock_.restart();
            sf::Packet pingPacket;
            pingPacket << PING;
            socket_.send(pingPacket);
        }
        else if(messageType == CHAT_MESSAGE)
        {
            std::string playerName;
            std::string message;
            packet >> playerName >> message;
            sf::Packet messagePacket;
            messagePacket << CHAT_MESSAGE << playerName << message;
            socket_.send(messagePacket);
        }
        else if (messageType == CLIENT_DATA)
        {
            sf::Int32 id;
            b2Transform transform;
            b2Vec2 velocity;
            float angularVelocity;
            packet >> id >> transform >> velocity >> angularVelocity;
            sf::Packet sendPacket;
            sendPacket << CLIENT_DATA << id << transform << velocity << angularVelocity;
            socket_.send(sendPacket);
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
    // Loop through all data on socket
    sf::Socket::Status status = sf::Socket::Done;
    while (status == sf::Socket::Done)
    {
        // Wait for data on socket
        sf::Packet packet;
        status = ReceiveIfReady(packet);
        if(status == sf::Socket::Done)
        {
            // Received data in packet
            NetworkMessageType messageType;
            if(packet >> messageType)
            {
                if(messageType == PING)
                {
                    sf::Time ping = pingClock_.getElapsedTime();
                    std::stringstream ss;
                    ss << std::fixed << std::setprecision(4) << ping.asSeconds();
                    ss << " seconds";
                    sf::Packet sendPacket;
                    sendPacket << PING << ss.str();
                    sceneManager_->HandlePacket(sendPacket);
                }
                else if (messageType == CHAT_MESSAGE)
                {
                    std::string playerName;
                    std::string message;
                    packet >> playerName >> message;
                    sf::Packet sendPacket;
                    sendPacket << CHAT_MESSAGE << playerName << message;
                    sceneManager_->HandlePacket(sendPacket);
                }
                else if (messageType == CLIENT_CONNECT)
                {
                    sf::Int32 numberOfClients;
                    packet >> numberOfClients;
                    for(int i = 0; i < numberOfClients; i++)
                    {
                        std::string clientName;
                        sf::Int32 id;
                        packet >> clientName >> id;
                        sf::Packet sendPacket;
                        sendPacket << CLIENT_CONNECT << clientName << id;
                        sceneManager_->HandlePacket(sendPacket);      
                    }
                }
                
                else if (messageType == CLIENT_DISCONNECT)
                {
                    std::string clientName;
                    sf::Int32 id;
                    packet >> clientName >> id;
                    sf::Packet sendPacket;
                    sendPacket << CLIENT_DISCONNECT << clientName << id;
                    sceneManager_->HandlePacket(sendPacket);
                }
                else if (messageType == GAME_START)
                {
                    sceneManager_->ChangeScene("game");
                }
                else if (messageType == CLIENT_ID)
                {
                    packet >> id_;
                }
                else if (messageType == CLIENT_DATA)
                {
                    sf::Int32 numberOfClients;
                    packet >> numberOfClients;
                    for(int i = 0; i < numberOfClients; i++)
                    {
                        sf::Int32 id;
                        b2Transform transform;
                        b2Vec2 velocity;
                        float angularVelocity;
                        packet >> id >> transform >> velocity >> angularVelocity;
                        sf::Packet sendPacket;
                        sendPacket << CLIENT_DATA << id << transform << velocity << angularVelocity;
                        sceneManager_->HandlePacket(sendPacket);      
                    }
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