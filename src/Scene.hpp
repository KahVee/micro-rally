#pragma once

#include <SFML/Graphics.hpp>

class Scene
{
public:
    virtual void HandleEvents(sf::RenderWindow& window) = 0;
    virtual void Update(const sf::Time& deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
};