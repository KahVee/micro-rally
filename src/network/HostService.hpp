#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <list>

#include "Network.hpp"

/*A class that represents the host-side of network communication.*/
class HostService
{
public:
    HostService() = default;
    ~HostService();
    HostService(const HostService& hostService) = delete;
    HostService& operator=(const HostService& hostService) = delete;
    sf::Socket::Status Listen(unsigned short port);
    void Start();
    void Stop();
    bool IsRunning();
private:
    void SendToAll(sf::Packet& packet);
    void SendToOne(sf::TcpSocket*& client, sf::Packet& packet);
    void RemoveClient(sf::TcpSocket*& client);
    sf::TcpListener listener_;
    sf::SocketSelector selector_;
    std::list<Client> clients_;
    bool running_ = false;
};