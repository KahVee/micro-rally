#pragma once

#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "Scene.hpp"
#include "Button.hpp"

class MainMenuScene : public Scene
{
public:
    MainMenuScene(const sf::Font& font, SceneManager& sceneManager);
    ~MainMenuScene() = default;
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
private:
    Button quit_;
    Button settings_;
};