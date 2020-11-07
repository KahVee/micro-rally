#include "ButtonSceneComponent.hpp"

ButtonSceneComponent::ButtonSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, std::function<void()> onClick) : SceneComponent(relativePosition, relativeSize, componentClass), onClick_(onClick), backgroundColor_(backgroundColor), highlightColor_(highlightColor), buttonSoundBuff_(buttonSoundBuff)
{
    // Set text
    text_.setString(text);
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(100);
    // Set RectangleShape
    rectangleShape_.setFillColor(backgroundColor);
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
    // Set button sound options
    buttonSound_.setBuffer(buttonSoundBuff_);
    buttonSound_.setVolume(30.f);
    
}

void ButtonSceneComponent::HandlePacket(sf::Packet packet){}

void ButtonSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            if(IsMouseHovering(window))
            {
                rectangleShape_.setFillColor(highlightColor_);
            }
            else
            {
                rectangleShape_.setFillColor(backgroundColor_);
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(IsMouseHovering(window))
            {
                onClick_();
                // Sound on click
                buttonSound_.play();
            }
            break;
        default:
            break;
    }
}

void ButtonSceneComponent::Update(const sf::Time& deltaTime){}

void ButtonSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
}

void ButtonSceneComponent::Init()
{
    rectangleShape_.setFillColor(backgroundColor_);
}

void ButtonSceneComponent::Reset(){}

void ButtonSceneComponent::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition(
        position.x + rectangleShape_.getGlobalBounds().width / 2.0f - text_.getGlobalBounds().width / 2.0f - text_.getGlobalBounds().left,
        position.y + rectangleShape_.getGlobalBounds().height / 2.0f - text_.getGlobalBounds().height / 2.0f - text_.getGlobalBounds().top
    );
    text_.setPosition(textPosition);
}

void ButtonSceneComponent::SetSize(const sf::Vector2f& size)
{
    rectangleShape_.setSize(size);
    float desiredScale = 0.75 / (text_.getLocalBounds().height / size.y);
    text_.setScale(desiredScale, desiredScale);
    if(text_.getGlobalBounds().width > size.x * 0.9)
    {
        desiredScale = 0.9 / (text_.getLocalBounds().width / size.x);
        text_.setScale(desiredScale, desiredScale);
    }
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