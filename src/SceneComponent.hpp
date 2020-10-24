#pragma once

#include <SFML/Graphics.hpp>

class SceneComponent
{
public:
    SceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize) : relativePosition_(relativePosition), relativeSize_(relativeSize) {}
    virtual ~SceneComponent() = default;
    virtual void HandleEvent(sf::Event& event, sf::RenderWindow& window) = 0;
    virtual void Update(const sf::Time& deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
protected:
    sf::Vector2f relativePosition_;
    sf::Vector2f relativeSize_;
};