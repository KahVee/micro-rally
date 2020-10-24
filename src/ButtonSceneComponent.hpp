#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

class ButtonSceneComponent : public SceneComponent
{
public:
    ButtonSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize, const sf::Color& backgroundColor, const sf::Color& highlightColor, std::function<void()> onClick);
    ~ButtonSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetPosition(const sf::Vector2f& position);
    void SetSize(const sf::Vector2f& size);
    void SetTextColor(const sf::Color& color);
    void SetBackgroundColor(const sf::Color& color);
    bool IsMouseHovering(sf::RenderWindow &window);
private:
    sf::Color backgroundColor_;
    sf::Color highlightColor_;
    sf::RectangleShape rectangleShape_;
    sf::Text text_;
    std::function<void()> onClick_;
};