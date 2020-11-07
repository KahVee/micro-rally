#include "TextSceneComponent.hpp"

#include <iostream>

TextSceneComponent::TextSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font) : SceneComponent(relativePosition, relativeSize, componentClass)
{
    // Set text
    text_.setString(text);
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(100);
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y}, {relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
}

void TextSceneComponent::HandlePacket(sf::Packet& packet){}

void TextSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window){}

void TextSceneComponent::Update(const sf::Time& deltaTime){}

void TextSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(text_);
}

void TextSceneComponent::Init(){}

void TextSceneComponent::Reset(){}

void TextSceneComponent::SetPosition(const sf::Vector2f& position, const sf::Vector2f& size)
{
    sf::Vector2f textPosition(
        (position.x + size.x / 2.0f) - text_.getGlobalBounds().width / 2.0f - text_.getGlobalBounds().left,
        (position.y + size.y / 2.0f) - text_.getGlobalBounds().height / 2.0f - text_.getGlobalBounds().top
    );
    text_.setPosition(textPosition);
}

void TextSceneComponent::SetSize(const sf::Vector2f& size)
{
    float desiredScale = 0.75 / (text_.getLocalBounds().height / size.y);
    text_.setScale(desiredScale, desiredScale);
    if(text_.getGlobalBounds().width > size.x * 0.9)
    {
        desiredScale = 0.9 / (text_.getLocalBounds().width / size.x);
        text_.setScale(desiredScale, desiredScale);
    }
}