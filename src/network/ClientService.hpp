#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <iomanip>
#include <sstream>

#include "../scene/SceneManager.hpp"

/*A class that represents the client-side of network communication.*/
class ClientService
{
public:
    ClientService() = default;
    ~ClientService() = default;
    void Init(SceneManager* sceneManager);
    sf::Socket::Status Connect(const sf::IpAddress &address, unsigned short port, sf::Time timeout, const std::string& playerName);
    void Disconnect();
    bool IsConnected();
    void Send(sf::Packet& packet);
    void Receive();
private:
    sf::Socket::Status ReceiveWithTimeout(sf::Packet& packet, sf::Time timeout);
    sf::TcpSocket socket_;
    sf::SocketSelector selector_;
    sf::Clock pingClock_;
    SceneManager* sceneManager_ = nullptr;
};