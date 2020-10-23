#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Button
{
public:
    Button() = default;
    Button(const sf::Vector2f& position, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize, const sf::Vector2f& size, const sf::Color& backgroundColor);
    ~Button() = default;
    void SetPosition(const sf::Vector2f& position);
    void SetTextColor(const sf::Color& color);
    void SetBackgroundColor(const sf::Color& color);
    void Draw(sf::RenderWindow& window);
    bool IsMouseHovering(sf::RenderWindow &window);
private:
    sf::RectangleShape rectangleShape_;
    sf::Text text_;
};