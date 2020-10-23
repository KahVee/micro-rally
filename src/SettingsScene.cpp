#include "SettingsScene.hpp"

SettingsScene::SettingsScene(const sf::Font& font, SceneManager& sceneManager) : 
    Scene(sceneManager),
    back_({100,300},"BACK", sf::Color::Blue, font, 25, {200, 50}, sf::Color::Green)
    {}

void SettingsScene::HandleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch(event.type)
        {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseMoved:
                if(back_.IsMouseHovering(window))
                {
                    back_.SetBackgroundColor(sf::Color::White);
                }
                else
                {
                    back_.SetBackgroundColor(sf::Color::Green);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if(back_.IsMouseHovering(window))
                {
                    sceneManager_.ChangeScene("mainmenu");
                }
                break;
            default:
                break;
        }
    }
}

void SettingsScene::Update(const sf::Time& deltaTime)
{
}
void SettingsScene::Draw(sf::RenderWindow& window)
{
    back_.Draw(window);
}