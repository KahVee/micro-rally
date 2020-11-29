#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../settings/Settings.hpp"
#include "Scene.hpp"
#include "TableSceneComponent.hpp"
#include "TextInputSceneComponent.hpp"
#include "../GameObject/Game.hpp"
#include "../GameObject/DynamicObject.hpp"
#include "../constants.hpp"
#include "../network/ClientService.hpp"

class ClientService;

/*A Scene that contains SceneComponents.*/
class GameScene : public Scene
{
public:
    GameScene(ClientService* clientService, sf::RenderWindow& window, const sf::Font& font, Settings& settings, const sf::Color& backgroundColor);
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
    TableSceneComponent chat_;
    TextInputSceneComponent textInput_;
    bool drawChat_ = false;
    TableSceneComponent playerList_;
    bool drawPlayerList_ = false;
    bool playerFinished_ = false;
    Settings* settings_;
};