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
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(settings.GetWidth(), settings.GetHeight()), "PhysicsTest", sf::Style::Titlebar | sf::Style::Close);//, contextSettings);

    // The font of the program
    sf::Font font;
    if(!font.loadFromFile("../res/FreeMono.ttf"))
    {
        std::cout << "font load error" << std::endl;
        window.close();
    }
    // Load texture for menu background
    sf::Texture menuBackgroundTexture;
    if (!menuBackgroundTexture.loadFromFile("../res/Lemon.jpg"))
    {
        std::cout << "texture load error" << std::endl;
        window.close();
    }
    // Load ButtonSound
    sf::SoundBuffer buttonSoundBuffer;
    if (!buttonSoundBuffer.loadFromFile("../res/ButtonSound.wav"))
    {
        std::cout << "audio load error" << std::endl;
        window.close();
    }
    //Load Theme1
    sf::Music theme1;
    if(!theme1.openFromFile("../res/Theme1.wav"))
    {
        std::cout << "audio load error" << std::endl;
        window.close();
    }
    theme1.setVolume(15.f);
    theme1.setLoop(true);
    
    // This object provides delta time
    sf::Clock clock;
    // The name of the local player
    std::string playerName = "player";
    // HostService provides host side networking run in a separate thread
    HostService hostService;
    std::thread hostThread;
    // ClientService provides client side networking
    ClientService clientService;
    // The object that manages scenes of the program
    SceneManager sceneManager;
    // Initialize SceneManager
    sceneManager.Init(hostService, clientService, hostThread, settings, window, font, menuBackgroundTexture, buttonSoundBuffer);
    // Initialize ClientService
    clientService.Init(&sceneManager);
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
        window.clear(sf::Color(200, 200, 200, 255));
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