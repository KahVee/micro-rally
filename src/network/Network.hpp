#pragma once
#include <SFML/Network.hpp>
#include <Box2D/Box2D.h>
#include "../constants.hpp"

struct Client
{
    sf::Int32 id;
    sf::TcpSocket* socket;
    std::string name;
    b2Transform transform;
    b2Vec2 velocity;
    float angularVelocity;
    float steeringAngle;
    bool finished;
    sf::Int32 ranking;
    sf::Int32 car;
};

struct NetworkDynamicObject
{
    b2Transform transform;
    b2Vec2 velocity;
    float angularVelocity;
};

enum NetworkMessageType : sf::Uint8
{
    PING,
    CHAT_MESSAGE,
    CLIENT_START,
    CLIENT_CONNECT,
    CLIENT_DISCONNECT,
    CLIENT_DATA,
    CLIENT_ID,
    CLIENT_WIN,
    CLIENT_RANK,
    CLIENT_CAR,
    OBJECT_CREATE,
    OBJECT_DATA,
    GAME_START,
    GAME_FINISH
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