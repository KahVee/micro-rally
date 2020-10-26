#include <iostream>
#include <string>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <thread>

#include "SceneManager.hpp"
#include "MenuScene.hpp"
#include "ButtonSceneComponent.hpp"
#include "TextSceneComponent.hpp"
#include "PictureSceneComponent.hpp"
#include "TextInputSceneComponent.hpp"
#include "TableSceneComponent.hpp"
#include "HostService.hpp"
#include "ClientService.hpp"

int main()
{
    // The window of the program
    sf::RenderWindow window(sf::VideoMode(800, 450), "testsfml", sf::Style::Titlebar | sf::Style::Close);
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
    // This object provides delta time
    sf::Clock clock;
    // Host and client services
    HostService hostService;
    ClientService clientService;
    std::thread hostThread;
    std::thread clientThread;
    // The object that manages scenes of the program
    SceneManager sceneManager;
    // Gray color
    sf::Color Gray(100,100,100,100);
    // Create main menu scene ------------------------------------------------------------------------------------------
    MenuScene* mainMenu = new MenuScene();
    mainMenu->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    mainMenu->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"2D CAR GAME", sf::Color::Red, font));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.2f}, {0.3f, 0.1f}, window,"HOST", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("hostScene");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, window,"JOIN", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("joinScene");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.6f}, {0.3f, 0.1f}, window,"SETTINGS", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("settings");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, window,"QUIT", sf::Color::Black, font, Gray, sf::Color::White, [&window](){window.close();}));
    sceneManager.AddScene("mainMenu", mainMenu);
    // Create settings scene ------------------------------------------------------------------------------------------
    MenuScene* settings = new MenuScene();
    settings->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    settings->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"SETTINGS", sf::Color::Red, font));
    settings->AddSceneComponent(new TextInputSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, window,"inputtextbox", sf::Color::Black, font, Gray, sf::Color::White, 10, [](const std::string& text){return "";}));
    settings->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("mainMenu");}));
    sceneManager.AddScene("settings", settings);
    // Create Host lobby scene ------------------------------------------------------------------------------------------
    MenuScene* hostScene = new MenuScene();
    hostScene->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    hostScene->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"HOST LOBBY", sf::Color::Red, font));
    TableSceneComponent* hostTable = new TableSceneComponent({0.05f, 0.2f}, {0.5f, 0.6f}, window, sf::Color::Black, font, Gray, 12, {10, 20});
    hostScene->AddSceneComponent(hostTable);
    hostScene->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.5f, 0.1f}, window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [hostTable, &clientService](const std::string& text){
            if(clientService.IsRunning())
            {
                sf::Packet* packet = new sf::Packet;
                *packet << "CHAT_MESSAGE" << text;
                clientService.Send(packet);
            }
            return "";
        }));
    hostScene->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("mainMenu");}));
    hostScene->AddSceneComponent(new ButtonSceneComponent({0.6f, 0.85f}, {0.2f, 0.1f}, window,"START", sf::Color::Black, font, Gray, sf::Color::White, 
        [&hostService, &hostThread, &clientService, &clientThread](){
            if(!hostService.IsRunning())
            {
                if(hostService.Listen(25000) != sf::Socket::Done)
                {
                    std::cout << "hostService.Listen error" << std::endl;
                }
                hostThread = std::thread(&HostService::Start, std::ref(hostService));
                if(clientService.Connect("localhost", 25000) != sf::Socket::Done)
                {
                    std::cout << "clientService.Connect error" << std::endl;
                }
                clientThread = std::thread(&ClientService::Start, std::ref(clientService));
            }
        }));
    sceneManager.AddScene("hostScene", hostScene);
    // Create client lobby scene ------------------------------------------------------------------------------------------
    MenuScene* joinScene = new MenuScene();
    joinScene->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, window, texture));
    joinScene->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, window,"HOST LOBBY", sf::Color::Red, font));
    TableSceneComponent* clientTable = new TableSceneComponent({0.05f, 0.2f}, {0.5f, 0.6f}, window, sf::Color::Black, font, Gray, 12, {10, 20});
    joinScene->AddSceneComponent(clientTable);
    joinScene->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.5f, 0.1f}, window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [clientTable, &clientService](const std::string& text){
            if(clientService.IsRunning())
            {
                sf::Packet* packet = new sf::Packet;
                *packet << "CHAT_MESSAGE" << text;
                clientService.Send(packet);
            }
            return "";
        }));
    joinScene->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, [&sceneManager](){sceneManager.ChangeScene("mainMenu");}));
    joinScene->AddSceneComponent(new ButtonSceneComponent({0.6f, 0.85f}, {0.2f, 0.1f}, window,"JOIN", sf::Color::Black, font, Gray, sf::Color::White, 
        [&clientService, &clientThread](){
            if(!clientService.IsRunning())
            {
                if(clientService.Connect("localhost", 25000) != sf::Socket::Done)
                {
                    std::cout << "clientService.Connect error" << std::endl;
                }
                clientThread = std::thread(&ClientService::Start, std::ref(clientService));
            }
        }));
    sceneManager.AddScene("joinScene", joinScene);
    // Set initial scene
    sceneManager.ChangeScene("mainMenu");
    // Client message functions
    clientService.AddMessageFunction("CHAT_MESSAGE", [hostTable, clientTable](sf::Packet& packet){
        std::string message;
        packet >> message;
        hostTable->AddRow({"Name", message});
        clientTable->AddRow({"Name", message});
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
    }
    // Terminate all threads
    hostService.Stop();
    clientService.Stop();
    if(hostThread.joinable())
    {
        hostThread.join();
    }
    if(clientThread.joinable())
    {
        clientThread.join();
    }
    return 0;
}