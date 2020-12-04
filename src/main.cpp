#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <thread>


#include "scene/SceneManager.hpp"

// INCLUDED IN GameScene already (circular dependency if uncommented)
//#include "constants.hpp"
//included in scenemanager already
//#include "settings/Settings.hpp"

int main()
{
    Settings settings;
    if (!settings.LoadSettings())
    {
        return 1;
    }
    //sf::ContextSettings contextSettings;
    //contextSettings.antialiasingLevel = 8; TO ENABLE ANTI-ALIASING UNCOMMENT THE LINES WITH CONTEXTSETTINGS
    int style = sf::Style::Titlebar | sf::Style::Close;
    sf::VideoMode videoMode;
    if(settings.GetFullscreen())
    {
        style = sf::Style::Fullscreen;
        videoMode = settings.GetFullscreenVideoMode();
    }
    else
    {
        videoMode = settings.GetWindowedVideoMode();
    }
    // The window of the program
    sf::RenderWindow window(videoMode, "Micro Rally", style);//sf::Style::Titlebar | sf::Style::Close);//, contextSettings);
    // The font of the program
    sf::Font font;
    if(!font.loadFromFile("../res/FreeMono.ttf"))
    {
        std::cout << "font load error" << std::endl;
        window.close();
    }
    // Load texture for menu background
    sf::Texture menuBackgroundTexture;
    if (!menuBackgroundTexture.loadFromFile("../res/car_background.png"))
    {
        std::cout << "texture load error" << std::endl;
        window.close();
    }
    // This object provides delta time
    sf::Clock clock;
    // HostService provides host side networking run in a separate thread
    HostService hostService;
    std::thread hostThread;
    // ClientService provides client side networking
    ClientService clientService;
    // The object that manages scenes of the program
    SceneManager sceneManager;
    // Initialize SceneManager
    sceneManager.Init(hostService, clientService, hostThread, settings, window, font, menuBackgroundTexture);
    // Initialize ClientService
    clientService.Init(&sceneManager, &settings);
    // Limit framerate to 60
    window.setVerticalSyncEnabled(true);
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
        window.clear();//sf::Color(200, 200, 200, 255));
        // Draw current scene
        sceneManager.Draw(window);
        // Display everything in the window
        window.display();
        // Handle received client messages
        clientService.Receive();
    }
    // Terminate all threads
    hostService.Stop();
    if(hostThread.joinable())
    {
        hostThread.join();
    }
    settings.SaveSettings();
    return 0;
}