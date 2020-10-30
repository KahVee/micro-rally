#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "Scene.hpp"

/*A SceneManager contains Scene objects and methods related to them.*/
class SceneManager
{
public:
    SceneManager() = default;
    SceneManager(const SceneManager& sceneManager) = delete;
    SceneManager& operator=(const SceneManager& sceneManager) = delete;
    ~SceneManager();
    void AddScene(const std::string& sceneName, Scene* scenePointer);
    void SetInitialScene(const std::string& sceneName);
    void ChangeScene(const std::string& sceneName);
    Scene* GetCurrentScene() const;
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);

private:
    std::map<std::string, Scene*> scenes_;
    Scene* currentScene_;
};