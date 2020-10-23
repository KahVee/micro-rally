#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Button.hpp"

class SettingsScene : public Scene
{
public:
    SettingsScene(const sf::Font& font, SceneManager& sceneManager);
    ~SettingsScene() = default;
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
private:
    Button back_;
};