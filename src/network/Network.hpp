#pragma once
#include <SFML/Network.hpp>
#include <Box2D/Box2D.h>

struct Client
{
    sf::Int32 id;
    sf::TcpSocket* socket;
    std::string name;
    b2Transform transform;
    b2Vec2 velocity;
    float angularVelocity;
};

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