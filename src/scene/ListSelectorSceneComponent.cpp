#include "ListSelectorSceneComponent.hpp"

ListSelectorSceneComponent::ListSelectorSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const sf::Color& textColor, const sf::Font& font, const sf::Color& buttonColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, int currentIndex, int listSize, int maxTextSize, std::function<std::string(float)>  onChange) : SceneComponent(relativePosition, relativeSize, componentClass), onChange_(onChange), backgroundColor_(buttonColor), highlightColor_(highlightColor), buttonSoundBuff_(buttonSoundBuff), currentIndex_(currentIndex), listSize_(listSize), maxTextSize_(maxTextSize)
{
    // Set text
    text_.setString(onChange_(currentIndex_));
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(100);
    // Set RectangleShapes
    left_.setFillColor(buttonColor);
    middle_.setFillColor(buttonColor);
    right_.setFillColor(buttonColor);
    // Character needed for sizing
    character_.setString(" ");
    character_.setFont(font);
    character_.setCharacterSize(text_.getCharacterSize());
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y}, {relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    // Set button sound options
    buttonSound_.setBuffer(buttonSoundBuff_);
    buttonSound_.setVolume(30.f);
    
}

void ListSelectorSceneComponent::HandlePacket(sf::Packet packet){}

void ListSelectorSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            if(IsMouseHoveringLeft(window))
            {
                left_.setFillColor(highlightColor_);
            }
            else
            {
                left_.setFillColor(backgroundColor_);
            }
            if(IsMouseHoveringRight(window))
            {
                right_.setFillColor(highlightColor_);
            }
            else
            {
                right_.setFillColor(backgroundColor_);
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(IsMouseHoveringLeft(window))
                {
                    currentIndex_ -= 1;
                    if(currentIndex_ < 0)
                    {
                        currentIndex_ = listSize_ - 1;
                    }
                    text_.setString(onChange_(currentIndex_));
                    // Sound on click
                    buttonSound_.play();
                }
                if(IsMouseHoveringRight(window))
                {
                    currentIndex_ += 1;
                    if(currentIndex_ >=  listSize_)
                    {
                        currentIndex_ = 0;
                    }
                    text_.setString(onChange_(currentIndex_));
                    // Sound on click
                    buttonSound_.play();
                }
            }
            break;
        default:
            break;
    }
}

void ListSelectorSceneComponent::Update(const sf::Time& deltaTime){}

void ListSelectorSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(left_);
    window.draw(middle_);
    window.draw(right_);
    window.draw(text_);
}

void ListSelectorSceneComponent::Init()
{
    left_.setFillColor(backgroundColor_);
    middle_.setFillColor(backgroundColor_);
    right_.setFillColor(backgroundColor_);
}

void ListSelectorSceneComponent::Reset(){}

void ListSelectorSceneComponent::SetPosition(const sf::Vector2f& position, const sf::Vector2f& size)
{
    sf::Vector2f leftPosition(
        position.x,
        position.y
    );
    sf::Vector2f middlePosition(
        position.x + 0.2f * size.x,
        position.y
    );
    sf::Vector2f rightPosition(
        position.x + 0.85f * size.x,
        position.y
    );
    sf::Vector2f textPosition(
        (position.x + 0.2f * size.x + 0.3f * size.x) - maxTextSize_ * character_.getGlobalBounds().width * 0.5f,
        (position.y + 0.5f * size.y) - 0.5f * (character_.getGlobalBounds().height + character_.getGlobalBounds().top)
    );
    left_.setPosition(leftPosition);
    middle_.setPosition(middlePosition);
    right_.setPosition(rightPosition);
    text_.setPosition(textPosition);
}

void ListSelectorSceneComponent::SetSize(const sf::Vector2f& size)
{
    sf::Vector2f leftSize(
        0.15f * size.x,
        size.y
    );
    sf::Vector2f middleSize(
        0.6f * size.x,
        size.y
    );
    sf::Vector2f rightSize(
        0.15f * size.x,
        size.y
    );
    left_.setSize(leftSize);
    middle_.setSize(middleSize);
    right_.setSize(rightSize);

    float desiredScale = 0.95f / ((character_.getLocalBounds().height + character_.getLocalBounds().top) / size.y);
    text_.setScale(desiredScale, desiredScale);
    character_.setScale(desiredScale, desiredScale);
    if(maxTextSize_ * character_.getGlobalBounds().width > (size.x * 0.6) * 0.9)
    {
        desiredScale = 0.95 / (maxTextSize_ * character_.getLocalBounds().width / (size.x * 0.6));
        text_.setScale(desiredScale, desiredScale);
        character_.setScale(desiredScale, desiredScale);
    }
}

bool ListSelectorSceneComponent::IsMouseHoveringLeft(sf::RenderWindow &window)
{
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float minX = left_.getPosition().x;
    float minY = left_.getPosition().y;
    float maxX = minX + left_.getLocalBounds().width;
    float maxY = minY + left_.getLocalBounds().height;
    if(minX < mouseX && mouseX < maxX && minY < mouseY && mouseY < maxY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ListSelectorSceneComponent::IsMouseHoveringRight(sf::RenderWindow &window)
{
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float minX = right_.getPosition().x;
    float minY = right_.getPosition().y;
    float maxX = minX + right_.getLocalBounds().width;
    float maxY = minY + right_.getLocalBounds().height;
    if(minX < mouseX && mouseX < maxX && minY < mouseY && mouseY < maxY)
    {
        return true;
    }
    else
    {
        return false;
    }
}