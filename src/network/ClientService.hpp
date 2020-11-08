#pragma once

#include <SFML/Network.hpp>
#include <Box2D/Box2D.h>
#include <string>
#include <iomanip>
#include <sstream>

#include "../scene/SceneManager.hpp"
#include "Network.hpp"

class SceneManager;

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
    sf::Int32 GetId() const;
private:
    sf::Socket::Status ReceiveIfReady(sf::Packet& packet);
    sf::TcpSocket socket_;
    sf::SocketSelector selector_;
    sf::Clock pingClock_;
    SceneManager* sceneManager_ = nullptr;
    sf::Int32 id_ = -1;
};