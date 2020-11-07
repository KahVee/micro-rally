#pragma once

#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a picture on screen.*/
class PictureSceneComponent : public SceneComponent
{
public:
    PictureSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, const std::string& componentClass, sf::RenderWindow& window, const sf::Texture& texture);
    ~PictureSceneComponent() = default;
    void HandlePacket(sf::Packet packet);
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
private:
    void SetPosition(const sf::Vector2f& position);
    void SetScale(const sf::Vector2f& scale);

    sf::Texture texture_;
    sf::Sprite sprite_;
};