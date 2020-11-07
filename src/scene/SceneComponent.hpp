#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <string>

/*A virtual class that represents a part of a Scene.*/
class SceneComponent
{
public:
    SceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass) : relativePosition_(relativePosition), relativeSize_(relativeSize), componentClass_(componentClass) {}
    virtual ~SceneComponent() = default;
    virtual void HandlePacket(sf::Packet& packet) = 0;
    virtual void HandleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void Update(const sf::Time& deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
    virtual void Init() = 0;
    virtual void Reset() = 0;
protected:
    sf::Vector2f relativePosition_;
    sf::Vector2f relativeSize_;
    std::string componentClass_;
};