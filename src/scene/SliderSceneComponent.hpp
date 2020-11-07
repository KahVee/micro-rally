#pragma once

#include <algorithm>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a clickable button on screen.*/
class SliderSceneComponent : public SceneComponent
{
public:
    SliderSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font, const sf::Color& buttonColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, const sf::Color& lineColor, float relativeButtonPosition, std::function<std::string(float)> onSlide);
    ~SliderSceneComponent() = default;
    void HandlePacket(sf::Packet& packet);
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
private:
    void SetPosition(const sf::Vector2f& position, const sf::Vector2f& size);
    void SetSize(const sf::Vector2f& size);
    bool IsMouseHovering(sf::RenderWindow &window);

    sf::Color backgroundColor_;
    sf::Color highlightColor_;
    sf::RectangleShape button_;
    sf::RectangleShape line_;
    std::string textString_;
    sf::Text text_;
    std::function<std::string(float)> onSlide_;
    sf::Sound buttonSound_;
    sf::SoundBuffer buttonSoundBuff_;
    float relativeButtonPosition_;
    bool selected_ = false;
};