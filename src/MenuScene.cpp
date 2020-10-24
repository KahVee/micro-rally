#include "MenuScene.hpp"

MenuScene::~MenuScene()
{
    for(auto sceneComponent : sceneComponents_)
    {
        delete sceneComponent;
    }
}

void MenuScene::HandleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        else
        {
            for(auto sceneComponent : sceneComponents_)
            {
                sceneComponent->HandleEvent(event, window);
            }
        }
    }
}

void MenuScene::Update(const sf::Time& deltaTime)
{
    for(auto sceneComponent : sceneComponents_)
    {
        sceneComponent->Update(deltaTime);
    }
}
void MenuScene::Draw(sf::RenderWindow& window)
{
    for(auto sceneComponent : sceneComponents_)
    {
        sceneComponent->Draw(window);
    }
}

void MenuScene::AddSceneComponent(SceneComponent* sceneComponent)
{
    sceneComponents_.push_back(sceneComponent);
}