#include "ClientService.hpp"

ClientService::ClientService()
{
    running_ = false;
}


ClientService::~ClientService()
{
    for(auto packet : packetQueue_)
    {
        delete packet;
    }
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

void ClientService::Start()
{
    {
        // Delete old packets
        const std::lock_guard<std::mutex> lock(packetQueueMutex_);
        for(auto packet : packetQueue_)
        {
            delete packet;
        }
        packetQueue_.clear();
    }
    running_ = true;
    while(running_)
    {
        // Wait for data on socket
        sf::Packet packet;
        if(ReceiveWithTimeout(packet, sf::seconds(0.05f)) == sf::Socket::Done)
        {
            // Received data in packet
            std::string messageType;
            if(packet >> messageType)
            {
                messageFunctions_[messageType](packet);
            }
            // Nothing implemented for failure to read            
        }
        {
            // Send packets in queue
            const std::lock_guard<std::mutex> lock(packetQueueMutex_);
            for(auto packet : packetQueue_)
            {
                socket_.send(*packet);
                delete packet;
            }
            packetQueue_.clear();
        }
    }
    // Reset clientservice
    selector_.clear();
    socket_.disconnect();
}

void ClientService::Stop()
{
    running_ = false;
}

bool ClientService::IsRunning()
{
    return running_;
}

void ClientService::Send(sf::Packet* packet)
{
    const std::lock_guard<std::mutex> lock(packetQueueMutex_);
    packetQueue_.push_back(packet);
}

void ClientService::AddMessageFunction(const std::string& messageType, std::function<void(sf::Packet& packet)> function)
{
    messageFunctions_[messageType] = function;
}

sf::Socket::Status ClientService::ReceiveWithTimeout(sf::Packet& packet, sf::Time timeout)
{   
    if (selector_.wait(timeout))
        return socket_.receive(packet);
    else
        return sf::Socket::NotReady;
}