#include "GameScene.hpp"

void GameScene::HandleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        // Handle events
    }
}

void GameScene::Update(const sf::Time& deltaTime)
{
    // Update things
}
void GameScene::Draw(sf::RenderWindow& window)
{
    // Draw things
}

void GameScene::Init()
{
    // This is called when the current scene is changed to this one
}

void GameScene::Reset()
{
    // This is called when the current scene is changed to another one from this
}