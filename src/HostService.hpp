#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <atomic>

/*A class that represents the host-side of network communication.*/
class HostService
{
public:
    HostService();
    ~HostService();
    HostService(const HostService& hostService) = delete;
    HostService& operator=(const HostService& hostService) = delete;
    sf::Socket::Status Listen(unsigned short port);
    void Start();
    void Stop();
    bool IsRunning();
private:
    sf::TcpListener listener_;
    sf::SocketSelector selector_;
    std::vector<sf::TcpSocket*> clients_;
    std::atomic<bool> running_;
};