#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "SceneManager.hpp"
#include "MainMenuScene.hpp"

int main()
{
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(800, 600), "testsfml");
    // The font of the program
    sf::Font font;
    if(!font.loadFromFile("FreeMono.ttf"))
    {
        std::cout << "font load error" << std::endl;
        window.close();
    }
    // This object provides delta time
    sf::Clock clock;
    // The object that manages scenes of the program
    SceneManager sceneManager("Main Menu", new MainMenuScene(font));
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