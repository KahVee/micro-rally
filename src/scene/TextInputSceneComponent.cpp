#include "TextInputSceneComponent.hpp"

TextInputSceneComponent::TextInputSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, const sf::Color& highlightColor, int characterLimit, std::function<std::string(std::string)> onSubmit) : SceneComponent(relativePosition, relativeSize, componentClass), backgroundColor_(backgroundColor), highlightColor_(highlightColor), characterLimit_(characterLimit), onSubmit_(onSubmit)
{
    // Set text
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
    text_.setCharacterSize(100);
    // Cursor settings
    cursor_.setString("_");
    cursor_.setFillColor(textColor);
    cursor_.setFont(font);
    cursor_.setCharacterSize(text_.getCharacterSize());
    cursorPosition_ = textString_.length();
    // RectangleShape settings
    rectangleShape_.setFillColor(backgroundColor);
    // Set size and position
    SetSize({relativeSize.x * window.getSize().x, relativeSize.y * window.getSize().y});
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
}

void TextInputSceneComponent::HandlePacket(sf::Packet packet){}

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
                    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                }
                else if(std::isprint(c) && unsubmittedTextString_.length() < characterLimit_)
                {
                    unsubmittedTextString_.insert(unsubmittedTextString_.begin() + cursorPosition_, c);
                    text_.setString(unsubmittedTextString_);
                    cursorPosition_++;
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
                    if(componentClass_ == "deselectonsubmit")
                    {
                        selected_ = false;
                        rectangleShape_.setFillColor(backgroundColor_);   
                    }
                    unsubmittedTextString_ = onSubmit_(unsubmittedTextString_);
                    textString_ = unsubmittedTextString_;
                    text_.setString(textString_);
                    cursorPosition_ = textString_.length();
                    cursor_.setPosition(text_.findCharacterPos(cursorPosition_));
                }
            }
            break;
        default:
            break;
    }
}

void TextInputSceneComponent::Update(const sf::Time& deltaTime){}

void TextInputSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(rectangleShape_);
    window.draw(text_);
    if(selected_)
    {
        window.draw(cursor_);
    }
}

void TextInputSceneComponent::Init(){}

void TextInputSceneComponent::Reset(){}

void TextInputSceneComponent::SetPosition(const sf::Vector2f& position)
{
    rectangleShape_.setPosition(position);
    sf::Vector2f textPosition(
        position.x + rectangleShape_.getGlobalBounds().width / 2.0f - characterLimit_ * cursor_.getGlobalBounds().width / 2.0f,
        position.y + rectangleShape_.getGlobalBounds().height / 2.0f - (cursor_.getGlobalBounds().height + cursor_.getGlobalBounds().top) / 2.0f 
    );
    text_.setPosition(textPosition);
}

void TextInputSceneComponent::SetSize(const sf::Vector2f& size)
{
    rectangleShape_.setSize(size);
    float desiredScale = 0.75 / ((cursor_.getLocalBounds().height + cursor_.getLocalBounds().top)/ size.y);
    text_.setScale(desiredScale, desiredScale);
    cursor_.setScale(desiredScale, desiredScale);
    if(characterLimit_ * cursor_.getGlobalBounds().width > size.x * 0.9)
    {
        desiredScale = 0.9 / (characterLimit_ * cursor_.getLocalBounds().width / size.x);
        text_.setScale(desiredScale, desiredScale);
        cursor_.setScale(desiredScale, desiredScale);
    }
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

void TextInputSceneComponent::SetSelected(bool selected)
{
    selected_ = selected;
}
bool TextInputSceneComponent::GetSelected()
{
    return selected_;
}