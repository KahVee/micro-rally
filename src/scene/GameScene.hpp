#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Scene.hpp"
#include "../GameObject/Game.hpp"
#include "../constants.hpp"

/*A Scene that contains SceneComponents.*/
class GameScene : public Scene
{
public:
    GameScene();
    GameScene(const GameScene& gameScene) = delete;
    GameScene& operator=(const GameScene& gameScene) = delete;
    ~GameScene();
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
    // Public members
private:
    Game *game_;
    sf::Music theme2_;
};