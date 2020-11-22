#pragma once
#include <SFML/Network.hpp>
#include <Box2D/Box2D.h>

#define MAX_CLIENTS 10

struct Client
{
    sf::Int32 id;
    sf::TcpSocket* socket;
    std::string name;
    b2Transform transform;
    b2Vec2 velocity;
    float angularVelocity;
    float steeringAngle;
};

enum NetworkMessageType : sf::Uint8
{
    PING,
    CHAT_MESSAGE,
    CLIENT_CONNECT,
    CLIENT_DISCONNECT,
    CLIENT_DATA,
    CLIENT_ID,
    GAME_START
};

inline sf::Packet& operator <<(sf::Packet& packet, const NetworkMessageType& networkMessageType)
{
    return packet << static_cast<sf::Uint8>(networkMessageType);
}

inline sf::Packet& operator >>(sf::Packet& packet, NetworkMessageType& networkMessageType)
{
    sf::Uint8 temp;
    packet >> temp;
    networkMessageType = static_cast<NetworkMessageType>(temp);
    return packet;
}

inline sf::Packet& operator <<(sf::Packet& packet, const b2Transform& transform)
{
    return packet << transform.p.x << transform.p.y << transform.q.c << transform.q.s;
}

inline sf::Packet& operator >>(sf::Packet& packet, b2Transform& transform)
{
    return packet >> transform.p.x >> transform.p.y >> transform.q.c >> transform.q.s;
}

inline sf::Packet& operator <<(sf::Packet& packet, const b2Vec2& vec2)
{
    return packet << vec2.x << vec2.y;
}

inline sf::Packet& operator >>(sf::Packet& packet, b2Vec2& vec2)
{
    return packet >> vec2.x >> vec2.y;
}