#pragma once

#include <SFML/Graphics.hpp>

#include "Scene.hpp"

/*A Scene that contains SceneComponents.*/
class GameScene : public Scene
{
public:
    GameScene() = default;
    GameScene(const GameScene& menuScene) = delete;
    GameScene& operator=(const GameScene& menuScene) = delete;
    ~GameScene() = default;
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
    // Public members
private:
    // Private members
};