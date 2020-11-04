#include <iostream>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <thread>
#include <iomanip>
#include <sstream>

#include "scene/SceneManager.hpp"
#include "scene/GameScene.hpp"
#include "constants.hpp"

int main()
{
    //sf::ContextSettings contextSettings;
    //contextSettings.antialiasingLevel = 8; TO ENABLE ANTI-ALIASING UNCOMMENT THE LINES WITH CONTEXTSETTINGS
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PhysicsTest", sf::Style::Titlebar | sf::Style::Close);//, contextSettings);

    sf::Clock clock;
    SceneManager sceneManager;
    GameScene* game = new GameScene();
    sceneManager.AddScene("game", game);
    // Set initial scene
    sceneManager.SetInitialScene("game");
    sf::Event event;
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
        window.clear(sf::Color(sf::Color(200, 200, 200, 255)));
        // Draw current scene
        sceneManager.Draw(window);
        // Display everything in the window
        window.display();
    }
    return 0;
}