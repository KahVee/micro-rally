#pragma once

#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a picture on screen.*/
class PictureSceneComponent : public SceneComponent
{
public:
    PictureSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const sf::Texture& texture);
    ~PictureSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
private:
    void SetPosition(const sf::Vector2f& position);
    void SetScale(const sf::Vector2f& scale);

    sf::Texture texture_;
    sf::Sprite sprite_;
};