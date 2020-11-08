#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Scene.hpp"
#include "../GameObject/Game.hpp"
#include "../GameObject/DynamicObject.hpp"
#include "../constants.hpp"
#include "../network/ClientService.hpp"

class ClientService;

/*A Scene that contains SceneComponents.*/
class GameScene : public Scene
{
public:
    GameScene(ClientService* clientService);
    GameScene(const GameScene& gameScene) = delete;
    GameScene& operator=(const GameScene& gameScene) = delete;
    ~GameScene();
    void HandlePacket(sf::Packet& packet);
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
private:
    Game *game_ = nullptr;
    sf::Music theme2_;
    ClientService* clientService_;
};