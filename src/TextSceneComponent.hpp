#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "SceneComponent.hpp"

/*A SceneComponent that displays text.*/
class TextSceneComponent : public SceneComponent
{
public:
    TextSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const std::string& text, const sf::Color& textColor, const sf::Font& font);
    ~TextSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
private:
    void SetPosition(const sf::Vector2f& position, const sf::Vector2f& size);
    void SetSize(const sf::Vector2f& size);
    
    sf::Text text_;
};