#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "Scene.hpp"
#include "SceneComponent.hpp"

/*A Scene that contains SceneComponents.*/
class MenuScene : public Scene
{
public:
    MenuScene() = default;
    MenuScene(const MenuScene& menuScene) = delete;
    MenuScene& operator=(const MenuScene& menuScene) = delete;
    ~MenuScene();
    void HandleEvents(sf::RenderWindow& window);
    void Update(const sf::Time& deltaTime);
    void Draw(sf::RenderWindow& window);
    void Init();
    void Reset();
    void AddSceneComponent(SceneComponent* sceneComponent);
private:
    std::vector<SceneComponent*> sceneComponents_;
};