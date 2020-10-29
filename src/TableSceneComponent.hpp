#pragma once

#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <deque>

#include "SceneComponent.hpp"

/*A SceneComponent that represents a table on screen.*/
class TableSceneComponent : public SceneComponent
{
public:
    TableSceneComponent(const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const sf::Color& textColor, const sf::Font& font, const sf::Color& backgroundColor, int rowLimit, const std::vector<int>& columnCharacterLimits);
    ~TableSceneComponent() = default;
    void HandleEvent(sf::Event& event, sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    int AddRow(const std::vector<std::string>& row);
    void RemoveIndex(int i);
    void ReplaceIndex(int i, const std::vector<std::string>& row);
private:
    void SetPosition(const sf::Vector2f& position);
    void SetSize(const sf::Vector2f& size);
    bool IsMouseHovering(sf::RenderWindow &window);
    void UpdateText();
    sf::RectangleShape rectangleShape_;
    sf::Text text_;
    sf::Text character_;
    std::vector<int> columnCharacterLimits_;
    int rowLimit_;
    int rowCharacterLimit_;
    std::deque<std::vector<std::string>> table_;
};