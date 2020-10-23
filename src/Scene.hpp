#pragma once

#include <SFML/Graphics.hpp>
#include "SceneManager.hpp"

// This needs to be here because of cyclic dependencies
class SceneManager;

class Scene
{
public:
    Scene(SceneManager& sceneManager) : sceneManager_(sceneManager){}
    virtual void HandleEvents(sf::RenderWindow& window) = 0;
    virtual void Update(const sf::Time& deltaTime) = 0;
    virtual void Draw(sf::RenderWindow& window) = 0;
protected:
    SceneManager& sceneManager_;
};