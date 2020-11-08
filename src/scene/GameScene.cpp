#include "GameScene.hpp"

#include <vector>
#include <iostream>

GameScene::GameScene(ClientService* clientService) : clientService_(clientService)
{
    // Load Theme2
    if(!theme2_.openFromFile("../res/boogiewoogiestomp.wav"))
    {
        std::cout << "audio load error" << std::endl;
    }
    theme2_.setVolume(15.f);
    theme2_.setLoop(true);
}

GameScene::~GameScene() {
    if(game_ != nullptr)
    {
        delete game_;
    }
}

void GameScene::HandlePacket(sf::Packet& packet)
{
    // TODO HANDLE CHAT, PING
    std::string messageType;
    packet >> messageType;
    if(messageType == "CHAT_MESSAGE")
    {
        // TODO
        // std::string playerName;
        // std::string message;
        // packet >> playerName >> message;
        // AddRow({playerName, message});
    }
    else if (messageType == "PING")
    {
        // TODO
        // std::string ping;
        // packet >> ping;
        // AddRow({"PING", ping});
    }
    else if(messageType == "CLIENT_CONNECT")
    {
        std::string clientName;
        sf::Int32 id;
        packet >> clientName >> id;
        // Do not update the players car
        if(clientService_->GetId() != id)
        {
            game_->AddCar(id, game_->CreateCar());
        }
    }
    else if(messageType == "CLIENT_DISCONNECT")
    {
        // TODO
        // std::string clientName;
        // sf::Int32 id;
        // packet >> clientName >> id;
        // ReplaceIndex(id, {"",""});
    }
    else if (messageType == "PLAYER_INFO")
    {
        // TODO
        sf::Int32 id;
        b2Transform transform;
        b2Vec2 velocity;
        float angularVelocity;
        packet >> id >> transform >> velocity >> angularVelocity;
        // Do not update the players car
        if(clientService_->GetId() != id)
        {
            game_->UpdateCar(id, transform, velocity, angularVelocity);
        }
        else
        {
            // Update player info on server
            sf::Packet sendPacket;
            sendPacket << "PLAYER_INFO" << clientService_->GetId() << game_->GetPlayerCar()->GetTransform() << game_->GetPlayerCar()->GetVelocity() << game_->GetPlayerCar()->GetAngularVelocity();
            clientService_->Send(sendPacket);
        }
    }
}

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
        if(event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::W:
                    game_->GetPlayerCar()->Accelerate(true);
                    break;
                case sf::Keyboard::S:
                    game_->GetPlayerCar()->Brake(true);
                    break;
                case sf::Keyboard::A:
                    game_->GetPlayerCar()->TurnLeft(true);
                    break;
                case sf::Keyboard::D:
                    game_->GetPlayerCar()->TurnRight(true);
                    break;
                default:
                    break;
            }
        }

        if(event.type == sf::Event::KeyReleased) {
            switch(event.key.code) {
                case sf::Keyboard::W:
                    game_->GetPlayerCar()->Accelerate(false);
                    break;
                case sf::Keyboard::S:
                    game_->GetPlayerCar()->Brake(false);
                    break;
                case sf::Keyboard::A:
                    game_->GetPlayerCar()->TurnLeft(false);
                    break;
                case sf::Keyboard::D:
                    game_->GetPlayerCar()->TurnRight(false);
                    break;
                default:
                    break;
            }
        }
    }
}

void GameScene::Update(const sf::Time& deltaTime)
{
    game_->Update(deltaTime.asSeconds());

    // This line prints the friction under the car
    // std::cout << game_->GetMap()->GetFriction(game_->GetPlayerCar()->GetTransform().p) << std::endl;
}
void GameScene::Draw(sf::RenderWindow& window)
{
    // Draw map and objects
    sf::View view(sf::FloatRect(0.f, 0.f, 640.f, 360.f));
    view.setCenter(game_->GetPlayerCar()->GetSprite().getPosition());
    //view.setRotation(game_->GetPlayerCar()->GetTransform().q.GetAngle() * -RAD_TO_DEG);
    window.setView(view);
    // Draw map
    window.draw(game_->GetMap()->GetMapDrawable());
    // Draw dynamic objects
    std::vector<DynamicObject*> objects = game_->GetObjects();
    for(auto o: objects) {
        window.draw(o->GetSprite());
    }
    // Draw minimap border
    window.setView(window.getDefaultView());
    sf::RectangleShape rectangle({window.getSize().x * 0.25f, window.getSize().y * 0.25f});
    rectangle.setPosition({window.getSize().x * 0.75f, 0.0f});
    rectangle.setOutlineThickness(5.0f);
    rectangle.setOutlineColor(sf::Color::Black);
    window.draw(rectangle);
    // Draw minimap
    sf::View minimapView(sf::FloatRect(0.f, 0.f, game_->GetMap()->GetWidth() * game_->GetMap()->GetTileSize() * PIXELS_PER_METER, game_->GetMap()->GetHeight() * game_->GetMap()->GetTileSize() * PIXELS_PER_METER));
    minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f)); // TODO Stop using magic numbers
    window.setView(minimapView);
    // Draw map
    window.draw(game_->GetMap()->GetMapDrawable());
    for(auto o: objects) {
        window.draw(o->GetSprite());
    }
    // Set default view back
    window.setView(window.getDefaultView());
}

// This is called when the current scene is changed to this one
void GameScene::Init()
{
    game_ = new Game();
    game_->GetPlayerCar()->Accelerate(false);
    game_->GetPlayerCar()->Brake(false);
    game_->GetPlayerCar()->TurnLeft(false);
    game_->GetPlayerCar()->TurnRight(false);
    theme2_.play();
}

// This is called when the current scene is changed to another one from this
void GameScene::Reset()
{
    theme2_.stop();
    if(game_ != nullptr)
    {
        delete game_;
        game_ = nullptr;
    }
}