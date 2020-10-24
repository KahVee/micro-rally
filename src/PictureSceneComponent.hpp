#pragma once

#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

class PictureSceneComponent : public SceneComponent
{
public:
    PictureSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const sf::Texture& texture);
    ~PictureSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void SetPosition(const sf::Vector2f& position);
    void SetScale(const sf::Vector2f& scale);
private:
    sf::Texture texture_;
    sf::Sprite sprite_;
};