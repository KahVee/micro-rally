#pragma once

#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a clickable button on screen.*/
class ButtonSceneComponent : public SceneComponent
{
public:
    ButtonSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, std::function<void()> onClick);
    ~ButtonSceneComponent() = default;
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
    std::function<void()> onClick_;
    sf::Sound buttonSound_;
    sf::SoundBuffer buttonSoundBuff_;
};