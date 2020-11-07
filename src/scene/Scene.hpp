#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

/*A virtual class that represents a view on screen with it's own event-handling, updating and drawing.*/
class Scene
{
public:
    virtual ~Scene() = default;
    virtual void HandlePacket(sf::Packet& packet) = 0;
    virtual void HandleEvents(sf::RenderWindow& window) = 0;
    virtual void Update(const sf::Time& deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
    virtual void Init() = 0;
    virtual void Reset() = 0;
};