#pragma once

#include <SFML/Network.hpp>
#include <deque>
#include <functional>
#include <map>
#include <atomic>
#include <string>

/*A class that represents the client-side of network communication.*/
class ClientService
{
public:
    ClientService() = default;
    ~ClientService();
    sf::Socket::Status Connect(const sf::IpAddress &address, unsigned short port);
    void Start();
    void Stop();
    bool IsRunning();
    void Send(sf::Packet* packet);
    void AddMessageFunction(const std::string& messageType, std::function<void(sf::Packet& packet)> function);
private:
    sf::Socket::Status ReceiveWithTimeout(sf::Packet& packet, sf::Time timeout);
    sf::TcpSocket socket_;
    sf::SocketSelector selector_;
    bool running_ = false;//std::atomic<bool> running_;
    std::deque<sf::Packet*> packetQueue_;
    std::map<std::string, std::function<void(sf::Packet& packet)>> messageFunctions_;
};