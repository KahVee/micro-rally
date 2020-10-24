#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "MenuScene.hpp"
#include "ButtonSceneComponent.hpp"
#include "TextSceneComponent.hpp"
#include "PictureSceneComponent.hpp"

int main()
{
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(800, 600), "testsfml", sf::Style::Titlebar | sf::Style::Close);
    // The font of the program
    sf::Font font;
    if(!font.loadFromFile("FreeMono.ttf"))
    {
        std::cout << "font load error" << std::endl;
        window.close();
    }
    // Load texture for menu background
    sf::Texture texture;
    if (!texture.loadFromFile("Lemon.jpg"))
    {
        std::cout << "texture load error" << std::endl;
        window.close();
    }
    // This object provides delta time
    sf::Clock clock;
    // The object that manages scenes of the program
    SceneManager sceneManager;
    // Gray color
    sf::Color Gray(100,100,100);
    // Create main menu scene
    MenuScene* mainMenu = new MenuScene(); // (const sf::Vector2f& relativePosition, const sf::Vector2f& relativeSize, sf::RenderWindow& window, const sf::Texture& texture)
    mainMenu->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    mainMenu->AddSceneComponent(new TextSceneComponent({0.4f, 0.0f}, {0.2f, 0.1f}, window,"2D CAR GAME", sf::Color::Red, font, 50u));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.4f, 0.2f}, {0.2f, 0.1f}, window,"QUIT", sf::Color::Black, font, 30u, Gray, sf::Color::White, [&window](){window.close();}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.4f, 0.4f}, {0.2f, 0.1f}, window,"SETTINGS", sf::Color::Black, font, 30u, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("settings");}));
    sceneManager.AddScene("mainmenu", mainMenu);
    // Create settings scene
    MenuScene* settings = new MenuScene();
    settings->AddSceneComponent(new ButtonSceneComponent({0.4f, 0.2f}, {0.2f, 0.1f}, window,"BACK", sf::Color::Black, font, 30u, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("mainmenu");}));
    sceneManager.AddScene("settings", settings);
    // Set initial scene
    sceneManager.ChangeScene("mainmenu");
    // Limit framerate to 60
    window.setFramerateLimit(60);
    // Main loop
    while (window.isOpen())
    {
        // Delta time
        sf::Time deltaTime = clock.restart();
        // Handle events
        sceneManager.HandleEvents(window);
        // Update scene
        sceneManager.Update(deltaTime);
        // Clear the window
        window.clear();
        // Draw current scene
        sceneManager.Draw(window);
        // Display everything in the window
        window.display();
    }
    return 0;
}