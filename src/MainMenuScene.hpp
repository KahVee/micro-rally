#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.hpp"
#include "Button.hpp"

class MainMenuScene : public Scene
{
public:
    MainMenuScene(const sf::Font& font);
    ~MainMenuScene() = default;
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
private:
    Button quit_;
};