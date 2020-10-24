#include "TextInputSceneComponent.hpp"

TextInputSceneComponent::TextInputSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const unsigned int& characterSize, const sf::Color& backgroundColor, const sf::Color& highlightColor, const int& characterLimit, std::function<std::string(std::string)> onSubmit) : SceneComponent(relativePosition, relativeSize), backgroundColor_(backgroundColor), highlightColor_(highlightColor), characterLimit_(characterLimit), onSubmit_(onSubmit)
{
    if(text.length() > characterLimit)
    {
        textString_ = text.substr(0, characterLimit);
        
    }
    else
    {
        textString_ = text;
    }
    unsubmittedTextString_ = textString_;
    text_.setString(textString_);
    text_.setFillColor(textColor);
    text_.setFont(font);
    text_.setCharacterSize(characterSize);
    // Cursor settings
    cursor_.setString("_");
    cursor_.setFillColor(textColor);
    cursor_.setFont(font);
    cursor_.setCharacterSize(characterSize);
    cursorPosition_ = textString_.length();
    rectangleShape_.setFillColor(backgroundColor);
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
}

void TextInputSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window)
{
    switch(event.type)
    {
        case sf::Event::MouseButtonPressed:
            if(IsMouseHovering(window))
            {
                selected_ = true;
                rectangleShape_.setFillColor(highlightColor_);
            }
            else
            {
                selected_ = false;
                rectangleShape_.setFillColor(backgroundColor_);
                unsubmittedTextString_ = textString_;
                text_.setString(textString_);
                SetPosition({relativePosition_.x * window.getSize().x, relativePosition_.y * window.getSize().y});
                cursorPosition_ = textString_.length();
                cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
            }
            break;
        case sf::Event::TextEntered:
            if(selected_ && event.text.unicode < 128)
            {
                char c = static_cast<char>(event.text.unicode);
                // If entered character is backspace
                if(c == '\b' && cursorPosition_ != 0)
                {
                    // Delete character at cursor position
                    unsubmittedTextString_.erase(unsubmittedTextString_.begin() + cursorPosition_ - 1);
                    text_.setString(unsubmittedTextString_);
                    cursorPosition_--;
                    SetPosition({relativePosition_.x * window.getSize().x, relativePosition_.y * window.getSize().y});
                    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                }
                else if(std::isprint(c) && unsubmittedTextString_.length() < characterLimit_)
                {
                    unsubmittedTextString_.insert(unsubmittedTextString_.begin() + cursorPosition_, c);
                    text_.setString(unsubmittedTextString_);
                    cursorPosition_++;
                    SetPosition({relativePosition_.x * window.getSize().x, relativePosition_.y * window.getSize().y});
                    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                }
            }
            break;
        case sf::Event::KeyPressed:
            if(selected_)
            {
                if(event.key.code == sf::Keyboard::Right)
                {
                    // Only increase position if not at end
                    if(cursorPosition_ < unsubmittedTextString_.length())
                    {
                        cursorPosition_++;
                        cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                    }
                }
                else if(event.key.code == sf::Keyboard::Left)
                {
                    // Only increase position if not at end
                    if(cursorPosition_ > 0)
                    {
                        cursorPosition_--;
                        cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    // When input is submitted
                    selected_ = false;
                    rectangleShape_.setFillColor(backgroundColor_);
                    unsubmittedTextString_ = onSubmit_(unsubmittedTextString_);
                    textString_ = unsubmittedTextString_;
                    text_.setString(textString_);
                    cursorPosition_ = textString_.length();
                    SetPosition({relativePosition_.x * window.getSize().x, relativePosition_.y * window.getSize().y});
                    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                }
            }
            break;
        default:
            break;
    }
}

void TextInputSceneComponent::Update(const sf::Time& deltaTime)
{
}

void TextInputSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
    if(selected_)
    {
        window.draw(cursor_);
    }
}

void TextInputSceneComponent::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition(
        (position.x + rectangleShape_.getGlobalBounds().width / 2) - (text_.getGlobalBounds().width / 2),
        (position.y + rectangleShape_.getGlobalBounds().height / 2) - (text_.getCharacterSize() / 1.5f)
    );
    text_.setPosition(textPosition);
}

void TextInputSceneComponent::SetSize(const sf::Vector2f& size)
{
    rectangleShape_.setSize(size);
}

void TextInputSceneComponent::SetTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}

void TextInputSceneComponent::SetBackgroundColor(const sf::Color& color)
{
    rectangleShape_.setFillColor(color);
}

bool TextInputSceneComponent::IsMouseHovering(sf::RenderWindow &window)
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