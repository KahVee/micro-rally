#include "PictureSceneComponent.hpp"

PictureSceneComponent::PictureSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const sf::Texture& texture) : SceneComponent(relativePosition, relativeSize), texture_(texture)
{
    // Set sprite
    sprite_.setTexture(texture_);
    sf::Vector2f scale(
        (relativeSize.x * window.getSize().x) / static_cast<float>(texture_.getSize().x),
        (relativeSize.y * window.getSize().y) / static_cast<float>(texture_.getSize().y)
    );
    // Set size and position
    SetScale(scale);
    SetPosition({relativePosition.x * window.getSize().x, relativePosition.y * window.getSize().y});
}

void PictureSceneComponent::HandleEvent(sf::Event& event, sf::RenderWindow& window){}

void PictureSceneComponent::Update(const sf::Time& deltaTime){}

void PictureSceneComponent::Draw(sf::RenderWindow& window)
{
    window.draw(sprite_);
}

void PictureSceneComponent::Init(){}

void PictureSceneComponent::Reset(){}

void PictureSceneComponent::SetPosition(const sf::Vector2f& position)
{
    sprite_.setPosition(position);
}

void PictureSceneComponent::SetScale(const sf::Vector2f& scale)
{
    sprite_.setScale(scale);
}