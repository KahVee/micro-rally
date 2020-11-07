#pragma once

#include <string>
#include <functional>
#include <cctype>
#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that allows input of text.*/
class TextInputSceneComponent : public SceneComponent
{
public:
    TextInputSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, const sf::Color& highlightColor, int characterLimit, std::function<std::string(std::string)> onSubmit);
    ~TextInputSceneComponent() = default;
    void HandlePacket(sf::Packet& packet);
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
private:
    void SetPosition(const sf::Vector2f& position);
    void SetSize(const sf::Vector2f& size);
    bool IsMouseHovering(sf::RenderWindow &window);

    sf::Color backgroundColor_;
    sf::Color highlightColor_;
    sf::RectangleShape rectangleShape_;
    sf::Text text_;
    std::string textString_;
    std::string unsubmittedTextString_;
    bool selected_ = false;
    int characterLimit_;
    int cursorPosition_;
    sf::Text cursor_;
    std::function<std::string(std::string)> onSubmit_;
};