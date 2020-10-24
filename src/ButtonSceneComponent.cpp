#include "ButtonSceneComponent.hpp"

#include <iostream>

ButtonSceneComponent::ButtonSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, const sf::Color& highlightColor, std::function<void()> onClick) : SceneComponent(relativePosition, relativeSize), onClick_(onClick), backgroundColor_(backgroundColor), highlightColor_(highlightColor)
{
    text_.setString(text);
    text_.setFillColor(textColor);
    text_.setFont(font);
    //text_.setCharacterSize(30);
    rectangleShape_.setFillColor(backgroundColor);
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
}

void ButtonSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            if(IsMouseHovering(window))
            {
                SetBackgroundColor(highlightColor_);
            }
            else
            {
                SetBackgroundColor(backgroundColor_);
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(IsMouseHovering(window))
            {
                onClick_();
            }
            break;
        default:
            break;
    }
}

void ButtonSceneComponent::Update(const sf::Time& deltaTime)
{
}

void ButtonSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
}

void ButtonSceneComponent::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition = {
        (position.x + rectangleShape_.getGlobalBounds().width / 2) - (text_.getGlobalBounds().width / 2),
        (position.y + rectangleShape_.getGlobalBounds().height / 2) - (text_.getGlobalBounds().height)
    };
    text_.setPosition(textPosition);
}

void ButtonSceneComponent::SetSize(const sf::Vector2f& size)
{
    rectangleShape_.setSize(size);
}

void ButtonSceneComponent::SetTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}

void ButtonSceneComponent::SetBackgroundColor(const sf::Color& color)
{
    rectangleShape_.setFillColor(color);
}

bool ButtonSceneComponent::IsMouseHovering(sf::RenderWindow &window)
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