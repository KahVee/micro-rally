#include "MainMenuScene.hpp"

MainMenuScene::MainMenuScene(const sf::Font& font, SceneManager& sceneManager) : 
    Scene(sceneManager),
    quit_({100,300},"QUIT", sf::Color::Blue, font, 25, {200, 50}, sf::Color::Green),
    settings_({300,100},"SETTINGS", sf::Color::Blue, font, 25, {200, 50}, sf::Color::Green)
    {}

void MainMenuScene::HandleEvents(sf::RenderWindow& window)
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
                if(quit_.IsMouseHovering(window))
                {
                    quit_.SetBackgroundColor(sf::Color::White);
                }
                else
                {
                    quit_.SetBackgroundColor(sf::Color::Green);
                }

                if(settings_.IsMouseHovering(window))
                {
                    settings_.SetBackgroundColor(sf::Color::White);
                }
                else
                {
                    settings_.SetBackgroundColor(sf::Color::Green);
                }
                break;
            case sf::Event::MouseButtonPressed:
                if(quit_.IsMouseHovering(window))
                {
                    window.close();
                }
                if(settings_.IsMouseHovering(window))
                {
                    sceneManager_.ChangeScene("settings");
                }
                break;
            default:
                break;
        }
    }
}

void MainMenuScene::Update(const sf::Time& deltaTime)
{
}
void MainMenuScene::Draw(sf::RenderWindow& window)
{
    quit_.Draw(window);
    settings_.Draw(window);
}