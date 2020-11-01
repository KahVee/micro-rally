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
#include "scene/MenuScene.hpp"
#include "scene/ButtonSceneComponent.hpp"
#include "scene/TextSceneComponent.hpp"
#include "scene/PictureSceneComponent.hpp"
#include "scene/TextInputSceneComponent.hpp"
#include "scene/TableSceneComponent.hpp"
#include "scene/SliderSceneComponent.hpp"
#include "network/HostService.hpp"
#include "network/ClientService.hpp"

int main()
{
    //sf::ContextSettings contextSettings;
    //contextSettings.antialiasingLevel = 8; TO ENABLE ANTI-ALIASING UNCOMMENT THE LINES WITH CONTEXTSETTINGS
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(1600, 900), "testsfml", sf::Style::Titlebar | sf::Style::Close);//, contextSettings);
    // The font of the program
    sf::Font font;
    if(!font.loadFromFile("../res/FreeMono.ttf"))
    {
        std::cout << "font load error" << std::endl;
        window.close();
    }
    // Load texture for menu background
    sf::Texture texture;
    if (!texture.loadFromFile("../res/Lemon.jpg"))
    {
        std::cout << "texture load error" << std::endl;
        window.close();
    }
    // Load ButtonSound
    sf::SoundBuffer buttonSoundBuff;
    if (!buttonSoundBuff.loadFromFile("../res/ButtonSound.wav"))
    {
        std::cout << "audio load error" << std::endl;
        window.close();
    }
    // Load Theme1
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
    // Ping clock
    sf::Clock pingClock;
    // The name of the local player
    std::string playerName = "player";
    // Host and client services
    HostService hostService;
    ClientService clientService;
    std::thread hostThread;
    // The object that manages scenes of the program
    SceneManager sceneManager;
    // Gray color
    sf::Color Gray(100,100,100,100);
    // Create main menu scene ------------------------------------------------------------------------------------------
    MenuScene* mainMenu = new MenuScene();
    mainMenu->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    mainMenu->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"2D CAR GAME", sf::Color::Red, font));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.2f}, {0.3f, 0.1f}, window,"HOST", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff,
        [&hostService, &hostThread, &clientService, &sceneManager, &theme1](){
            if(!hostService.IsRunning())
            {
                if(hostService.Listen(25000) == sf::Socket::Done)
                {
                    if(clientService.Connect("localhost", 25000, sf::seconds(5.0f)) == sf::Socket::Done)
                    {
                        hostThread = std::thread(&HostService::Start, std::ref(hostService));
                        clientService.Start();
                        theme1.play();
                        sceneManager.ChangeScene("lobby");
                    }
                    // TODO when client fails to connect
                }
            }
        }));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, window,"JOIN", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, [&sceneManager](){sceneManager.ChangeScene("join");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.6f}, {0.3f, 0.1f}, window,"SETTINGS", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, [&sceneManager](){sceneManager.ChangeScene("settings");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, window,"QUIT", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, [&window](){window.close();}));
    sceneManager.AddScene("mainMenu", mainMenu);
    // Create settings scene ------------------------------------------------------------------------------------------
    MenuScene* settings = new MenuScene();
    settings->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    settings->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"SETTINGS", sf::Color::Red, font));
    settings->AddSceneComponent(new TextInputSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, window, playerName, sf::Color::Black, font, Gray, sf::Color::White, 10,
        [&playerName](const std::string& text){
            playerName = text;
            return text;
        }));
    settings->AddSceneComponent(new SliderSceneComponent({0.35f, 0.6f}, {0.3f, 0.1f}, window,"Volume: ", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, sf::Color::Black, 0.5f,
        [](float relativeButtonPosition){
            float volume = relativeButtonPosition * 100.0f;
            sf::Listener::setGlobalVolume(volume);
            std::stringstream ss;
            ss << std::setw(3) << std::setfill(' ') << static_cast<int>(volume);
            return ss.str();
        }));
    settings->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, [&sceneManager](){sceneManager.ChangeScene("mainMenu");}));
    sceneManager.AddScene("settings", settings);
    // Create lobby scene ------------------------------------------------------------------------------------------
    MenuScene* lobby = new MenuScene();
    lobby->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    lobby->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"LOBBY", sf::Color::Red, font));
    TableSceneComponent* table = new TableSceneComponent({0.05f, 0.2f}, {0.5f, 0.6f}, window, sf::Color::Black, font, Gray, 12, {10, 20});
    lobby->AddSceneComponent(table);
    lobby->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.5f, 0.1f}, window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, &playerName, &pingClock](const std::string& text){
            if(clientService.IsRunning())
            {
                if(text == "/ping")
                {
                    // /ping command for debugging
                    pingClock.restart();
                    sf::Packet packet;
                    packet << "PING";
                    clientService.Send(packet);
                }
                else
                {
                    sf::Packet packet;
                    packet << "CHAT_MESSAGE" << playerName << text;
                    clientService.Send(packet);
                }
            }
            return "";
        }));
    lobby->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff,
        [&hostService, &hostThread, &clientService, &sceneManager, &theme1](){
            hostService.Stop();
            // Terminate thread if needed
            if(hostThread.joinable())
            {
                hostThread.join();
            }
            clientService.Stop();
            theme1.stop();
            sceneManager.ChangeScene("mainMenu");
        }));
    sceneManager.AddScene("lobby", lobby);
    // Create join scene ------------------------------------------------------------------------------------------
    MenuScene* join = new MenuScene();
    join->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    join->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"JOIN", sf::Color::Red, font));
    join->AddSceneComponent(new TextInputSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, &sceneManager, &theme1](const std::string& text){
            if(!clientService.IsRunning())
            {
                if(clientService.Connect(text, 25000, sf::seconds(5.0f)) == sf::Socket::Done)
                {
                    clientService.Start();
                    theme1.play();
                    sceneManager.ChangeScene("lobby");
                }
            }
            return text;
        }));
    join->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuff, [&sceneManager](){sceneManager.ChangeScene("mainMenu");}));
    sceneManager.AddScene("join", join);
    // Set initial scene
    sceneManager.SetInitialScene("mainMenu");
    // Client message functions
    clientService.AddMessageFunction("PING", [table, &pingClock](sf::Packet& packet){
        sf::Time ping = pingClock.restart();
        std::stringstream ss;
        ss << std::fixed << std::setprecision(4) << ping.asSeconds();
        ss << " seconds";
        table->AddRow({"PING", ss.str()});
    });
    clientService.AddMessageFunction("CHAT_MESSAGE", [table](sf::Packet& packet){
        std::string playerName;
        std::string message;
        packet >> playerName >> message;
        table->AddRow({playerName, message});
    });
    clientService.AddMessageFunction("DISCONNECT", [&clientService, &sceneManager, &theme1](sf::Packet& packet){
        theme1.stop();
        sceneManager.ChangeScene("mainMenu");
    });
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
        // Handle received client messages
        clientService.Receive();
    }
    // Terminate all threads
    hostService.Stop();
    if(hostThread.joinable())
    {
        hostThread.join();
    }
    return 0;
}