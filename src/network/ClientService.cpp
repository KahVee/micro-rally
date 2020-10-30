#include "ClientService.hpp"

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
    running_ = true;
}

void ClientService::Stop()
{
    // Reset clientservice
    selector_.clear();
    socket_.disconnect();
    running_ = false;
    sf::Packet packet;
    messageFunctions_["DISCONNECT"](packet);
}

bool ClientService::IsRunning()
{
    return running_;
}

void ClientService::Send(sf::Packet& packet)
{
    // Check if client connected
    if(socket_.getLocalPort())
    {
        socket_.send(packet);
    }
    else
    {
        // If not connected reset and set running_ = false
        Stop();
    }
}

void ClientService::Receive()
{
    if(running_)
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
                messageFunctions_[messageType](packet);
            }        
        }
        else if(status == sf::Socket::Disconnected)
        {
            Stop();
        }
    }
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