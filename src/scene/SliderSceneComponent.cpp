#include "SliderSceneComponent.hpp"

SliderSceneComponent::SliderSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& buttonColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, const sf::Color& lineColor, float relativeButtonPosition, std::function<std::string(float)>  onSlide) : SceneComponent(relativePosition, relativeSize, componentClass), onSlide_(onSlide), backgroundColor_(buttonColor), highlightColor_(highlightColor), buttonSoundBuff_(buttonSoundBuff), relativeButtonPosition_(relativeButtonPosition), textString_(text)
{
    // Set text
    text_.setString(textString_ + onSlide_(relativeButtonPosition_));
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(100);
    // Set RectangleShapes§
    button_.setFillColor(buttonColor);
    line_.setFillColor(lineColor);
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y}, {relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    // Set button sound options
    buttonSound_.setBuffer(buttonSoundBuff_);
    buttonSound_.setVolume(30.f);
    
}

void SliderSceneComponent::HandlePacket(sf::Packet packet){}

void SliderSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    switch(event.type)
    {
        case sf::Event::MouseMoved:
            if(IsMouseHovering(window))
            {
                button_.setFillColor(highlightColor_);
            }
            else
            {
                button_.setFillColor(backgroundColor_);
            }
            if(selected_)
            {
                sf::Vector2f position(
                    relativePosition_.x * window.getSize().x,
                    relativePosition_.y * window.getSize().y
                );
                sf::Vector2f size(
                    relativeSize_.x * window.getSize().x,
                    relativeSize_.y * window.getSize().y
                );
                relativeButtonPosition_ = (event.mouseMove.x - position.x - 0.05f * size.x) / (size.x * 0.9f);
                relativeButtonPosition_ = std::max(0.0f, std::min(1.0f, relativeButtonPosition_));
                // Set text to new value
                text_.setString(textString_ + onSlide_(relativeButtonPosition_));
                // Set position
                sf::Vector2f buttonPosition(
                    position.x + relativeButtonPosition_ * (0.9f * size.x),
                    position.y + 0.5f * size.y 
                );
                button_.setPosition(buttonPosition);
            }
            break;
        case sf::Event::MouseButtonPressed:
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                if(IsMouseHovering(window))
                {
                    selected_ = true;
                    // Sound on click
                    buttonSound_.play();
                }
            }
            break;
        case sf::Event::MouseButtonReleased:
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                selected_ = false;
            }
            break;
        default:
            break;
    }
}

void SliderSceneComponent::Update(const sf::Time& deltaTime){}

void SliderSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(line_);
    window.draw(button_);
    window.draw(text_);
}

void SliderSceneComponent::Init()
{
    button_.setFillColor(backgroundColor_);
}

void SliderSceneComponent::Reset(){}

void SliderSceneComponent::SetPosition(const sf::Vector2f& position, const sf::Vector2f& size)
{
    sf::Vector2f buttonPosition(
        position.x + relativeButtonPosition_ * (0.9f * size.x),
        position.y + 0.5f * size.y 
    );
    sf::Vector2f linePosition(
        position.x,
        position.y + 0.7f * size.y
    );
    sf::Vector2f textPosition(
        (position.x + 0.5f * size.x) - 0.5f * text_.getGlobalBounds().width - text_.getGlobalBounds().left,
        (position.y + 0.25f * size.y) - 0.5f * text_.getGlobalBounds().height - text_.getGlobalBounds().top
    );
    button_.setPosition(buttonPosition);
    line_.setPosition(linePosition);
    text_.setPosition(textPosition);
}

void SliderSceneComponent::SetSize(const sf::Vector2f& size)
{
    sf::Vector2f buttonSize(
        0.1f * size.x,
        0.5f * size.y
    );
    sf::Vector2f lineSize(
        size.x,
        0.1f * size.y
    );
    button_.setSize(buttonSize);
    line_.setSize(lineSize);
    float desiredScale = 0.75 / (text_.getLocalBounds().height / (0.5f * size.y));
    text_.setScale(desiredScale, desiredScale);
    if(text_.getGlobalBounds().width > size.x * 0.9)
    {
        desiredScale = 0.9 / (text_.getLocalBounds().width / size.x);
        text_.setScale(desiredScale, desiredScale);
    }
}

bool SliderSceneComponent::IsMouseHovering(sf::RenderWindow &window)
{
    float mouseX = sf::Mouse::getPosition(window).x;
    float mouseY = sf::Mouse::getPosition(window).y;
    float minX = button_.getPosition().x;
    float minY = button_.getPosition().y;
    float maxX = minX + button_.getLocalBounds().width;
    float maxY = minY + button_.getLocalBounds().height;
    if(minX < mouseX && mouseX < maxX && minY < mouseY && mouseY < maxY)
    {
        return true;
    }
    else
    {
        return false;
    }
}