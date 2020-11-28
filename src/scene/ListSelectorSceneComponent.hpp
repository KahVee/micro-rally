#pragma once

#include <algorithm>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a selector that selects from a list on screen.*/
class ListSelectorSceneComponent : public SceneComponent
{
public:
    ListSelectorSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const sf::Color& textColor, const sf::Font& font, const sf::Color& buttonColor, const sf::Color& highlightColor, const sf::SoundBuffer& buttonSoundBuff, int currentIndex, int listSize, int maxTextSize, std::function<std::string(float)> onChange);
    ~ListSelectorSceneComponent() = default;
    void HandlePacket(sf::Packet packet);
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
private:
    void SetPosition(const sf::Vector2f& position, const sf::Vector2f& size);
    void SetSize(const sf::Vector2f& size);
    bool IsMouseHoveringLeft(sf::RenderWindow &window);
    bool IsMouseHoveringRight(sf::RenderWindow &window);

    sf::Color backgroundColor_;
    sf::Color highlightColor_;
    sf::RectangleShape left_;
    sf::RectangleShape middle_;
    sf::RectangleShape right_;
    sf::Text text_;
    sf::Text character_;
    std::function<std::string(int)> onChange_;
    sf::Sound buttonSound_;
    sf::SoundBuffer buttonSoundBuff_;
    int listSize_;
    int currentIndex_;
    int maxTextSize_;
};