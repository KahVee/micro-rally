#pragma once

#include <SFML/Network.hpp>
#include <functional>
#include <map>
#include <string>

/*A class that represents the client-side of network communication.*/
class ClientService
{
public:
    ClientService() = default;
    ~ClientService() = default;
    sf::Socket::Status Connect(const sf::IpAddress &address, unsigned short port, sf::Time timeout);
    void Disconnect();
    bool IsConnected();
    void Send(sf::Packet& packet);
    void Receive();
    void AddMessageFunction(const std::string& messageType, std::function<void(sf::Packet& packet)> function);
private:
    sf::Socket::Status ReceiveWithTimeout(sf::Packet& packet, sf::Time timeout);
    sf::TcpSocket socket_;
    sf::SocketSelector selector_;
    std::map<std::string, std::function<void(sf::Packet& packet)>> messageFunctions_;
};