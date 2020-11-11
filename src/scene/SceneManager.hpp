#pragma once

#include <map>
#include <string>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include "Scene.hpp"
#include "GameScene.hpp"
#include "MenuScene.hpp"
#include "ButtonSceneComponent.hpp"
#include "TextSceneComponent.hpp"
#include "PictureSceneComponent.hpp"
#include "TextInputSceneComponent.hpp"
#include "TableSceneComponent.hpp"
#include "SliderSceneComponent.hpp"
#include "../network/HostService.hpp"
#include "../network/ClientService.hpp"
#include "../settings/Settings.hpp"

class ClientService;

/*A SceneManager contains Scene objects and methods related to them.*/
class SceneManager
{
public:
    SceneManager() = default;
    SceneManager(const SceneManager& sceneManager) = delete;
    SceneManager& operator=(const SceneManager& sceneManager) = delete;
    ~SceneManager();
    void Init(HostService& hostService, ClientService& clientService, std::thread& hostThread, Settings& settings, sf::RenderWindow& window, sf::Font& font, sf::Texture& menuBackgroundTexture, sf::SoundBuffer& buttonSoundBuffer);
    void AddScene(const std::string& sceneName, Scene* scenePointer);
    void SetInitialScene(const std::string& sceneName);
    void ChangeScene(const std::string& sceneName);
    Scene* GetCurrentScene() const;
    void HandlePacket(sf::Packet& packet);
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    std::map<std::string, Scene*> scenes_;
    Scene* currentScene_;
};