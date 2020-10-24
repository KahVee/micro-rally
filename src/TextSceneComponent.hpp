#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

class TextSceneComponent : public SceneComponent
{
public:
    TextSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize);
    ~TextSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetPosition(const sf::Vector2f& position, const sf::Vector2f& size);
    void SetCharacterSize(const unsigned int& size);
    void SetTextColor(const sf::Color& color);
private:
    sf::Text text_;
};