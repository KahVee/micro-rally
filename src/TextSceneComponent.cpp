#include "TextSceneComponent.hpp"

#include <iostream>

TextSceneComponent::TextSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize) : SceneComponent(relativePosition, relativeSize)
{
    text_.setString(text);
    text_.setFillColor(textColor);
    text_.setFont(font);
    //text_.setCharacterSize(30);
    SetCharacterSize(characterSize);
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y}, {relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
}

void TextSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
}

void TextSceneComponent::Update(const sf::Time& deltaTime)
{
}

void TextSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(text_);
}

void TextSceneComponent::SetPosition(const sf::Vector2f& position, const sf::Vector2f& size)
{
    sf::Vector2f textPosition = {
        (position.x + size.x / 2) - (text_.getGlobalBounds().width / 2),
        (position.y + size.y / 2) - (text_.getGlobalBounds().height / 2)
    };
    text_.setPosition(textPosition);
}

void TextSceneComponent::SetCharacterSize(const unsigned int& size)
{
    text_.setCharacterSize(size);
}

void TextSceneComponent::SetTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}