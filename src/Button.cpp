#include "Button.hpp"

Button::Button(const sf::Vector2f& position, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize, const sf::Vector2f& size, const sf::Color& backgroundColor)
{
    text_.setString(text);
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(characterSize);
    rectangleShape_.setSize(size);
    rectangleShape_.setFillColor(backgroundColor);
    SetPosition(position);
}

void Button::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition = {
        (position.x + rectangleShape_.getGlobalBounds().width / 2) - (text_.getGlobalBounds().width / 2),
        (position.y + rectangleShape_.getGlobalBounds().height / 2) - (text_.getGlobalBounds().height)
    };
    text_.setPosition(textPosition);
}

void Button::SetTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}

void Button::SetBackgroundColor(const sf::Color& color)
{
    rectangleShape_.setFillColor(color);
}

void Button::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
}

bool Button::IsMouseHovering(sf::RenderWindow &window)
{
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float minX = rectangleShape_.getPosition().x;
    float minY = rectangleShape_.getPosition().y;
    float maxX = minX + rectangleShape_.getLocalBounds().width;
    float maxY = minY + rectangleShape_.getLocalBounds().height;
    if(minX < mouseX && mouseX < maxX && minY < mouseY && mouseY < maxY)
    {
        return true;
    }
    else
    {
        return false;
    }
}