#include "GameScene.hpp"

GameScene::GameScene(ClientService* clientService, sf::RenderWindow& window, const sf::Font& font, Settings& settings, const sf::Color& backgroundColor) : clientService_(clientService), settings_(&settings),
    chat_({0.05f, 0.05f}, {0.4f, 0.75f}, "chat", window, sf::Color::Black, font, backgroundColor, 15, {10, 20}),
    playerList_({0.3f, 0.2f}, {0.4f, 0.6f}, "playerlist", window, sf::Color::Black, font, backgroundColor, MAX_CLIENTS, {3, 10}),
    textInput_({0.05f, 0.85f}, {0.4f, 0.1f}, "", window,"", sf::Color::Black, font, backgroundColor, sf::Color::White, 20,
        [clientService, &settings, this, &window](const std::string& text){
            if(clientService->IsConnected())
            {
                if(text.size() > 0 && text.at(0) == '/')
                {
                    if(text == "/ping")
                    {
                        // /ping command for debugging
                        sf::Packet packet;
                        packet << PING;
                        clientService->Send(packet);
                    }
                    else if(text == "/quit")
                    {
                        window.close();
                    }
                    else if(text == "/win")
                    {
                        // WIN COMMAND FOR DEBUGGING
                        sf::Packet packet;
                        packet << CLIENT_WIN;
                        clientService->Send(packet);
                    }
                    else
                    {
                        this->chat_.AddRow({"ERROR", "Invalid command!"});
                    }
                }
                else
                {
                    sf::Packet packet;
                    packet << CHAT_MESSAGE << settings.GetName() << text;
                    clientService->Send(packet);
                }
            }
            return "";
        })
{
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
    NetworkMessageType messageType;
    packet >> messageType;
    if(messageType == CHAT_MESSAGE)
    {
        std::string playerName;
        std::string message;
        packet >> playerName >> message;
        chat_.AddRow({playerName, message});
    }
    else if (messageType == PING)
    {
        std::string ping;
        packet >> ping;
        chat_.AddRow({"PING", ping});
    }
    else if(messageType == CLIENT_START)
    {   
        std::string clientName;
        sf::Int32 id = -1;
        sf::Int32 car = -1;
        packet >> clientName >> id >> car;
        // Add to the list of players
        playerList_.ReplaceIndex(id, {std::to_string(id), clientName});
        // Do not update the players car
        if(clientService_->GetId() != id)
        {
            game_->AddCar(id, settings_->GetCarNames()[car]);
        }
    }
    else if(messageType == CLIENT_DISCONNECT)
    {
        std::string clientName;
        sf::Int32 id = -1;
        packet >> clientName >> id;
        // Remove from the list of players
        playerList_.ReplaceIndex(id, {"",""});
        // Do not update the players car
        if(clientService_->GetId() != id)
        {
            if(game_->ContainsCar(id))
            {
                game_->RemoveCar(id);
            }
        }
    }
    else if (messageType == CLIENT_DATA)
    {
        sf::Int32 id = -1;
        b2Transform transform;
        transform.SetIdentity();
        b2Vec2 velocity;
        velocity.SetZero();
        float angularVelocity = 0.0f;
        float steeringAngle = 0.0f;
        packet >> id >> transform >> velocity >> angularVelocity >> steeringAngle;
        // Do not update the players car
        if(clientService_->GetId() != id)
        {
            game_->UpdateCar(id, transform, velocity, angularVelocity, steeringAngle);
        }
        else
        {
            // Update player info on server
            sf::Packet sendPacket;
            sendPacket << CLIENT_DATA << clientService_->GetId() << game_->GetPlayerCar()->GetTransform() << game_->GetPlayerCar()->GetVelocity() << game_->GetPlayerCar()->GetAngularVelocity() << game_->GetPlayerCar()->GetSteeringAngle();
            clientService_->Send(sendPacket);
            // If client is host send gameobject data to hostservice
            if(clientService_->GetId() == 0)
            {
                for(auto& object : game_->GetNetworkedObjects())
                {
                    sf::Packet sendPacket;
                    sendPacket << OBJECT_DATA << object->GetID() << object->GetTransform() << object->GetVelocity() << object->GetAngularVelocity();
                    clientService_->Send(sendPacket);
                }
            }
        }
    }
    else if (messageType == OBJECT_DATA)
    {
        // If client is host don't update
        if(clientService_->GetId() != 0)
        {
            sf::Int32 id = -1;
            b2Transform transform;
            transform.SetIdentity();
            b2Vec2 velocity;
            velocity.SetZero();
            float angularVelocity = 0.0f;
            packet >> id >> transform >> velocity >> angularVelocity;
            game_->UpdateObject(id, transform, velocity, angularVelocity);
        }
    }
    else if (messageType == CLIENT_WIN)
    {
        sf::Int32 id = -1;
        packet >> id;
        if(id == clientService_->GetId())
        {
            playerFinished_ = true;
        }
        else
        {
            if(game_->ContainsCar(id))
            {
                game_->RemoveCar(id);
            }
        }
    }
    else if (messageType == GAME_START)
    {
        gameStartClock_.restart();
        sf::Int32 map = -1;
        sf::Int32 laps = -1;
        packet >> map >> laps;
        settings_->SetLaps(laps);
        settings_->SetMapIndex(map);
        game_ = new Game(clientService_->GetId(), settings_, laps, settings_->GetCarNames()[settings_->GetCarIndex()], "../res/maps/" + settings_->GetMapNames()[map] + ".json");
        // Send data to initialize networked dynamic objects on host if client is host
        if(clientService_->GetId() == 0)
        {
            for(auto& object : game_->GetNetworkedObjects())
            {
                sf::Packet sendPacket;
                sendPacket << OBJECT_CREATE << object->GetID() << object->GetTransform() << object->GetVelocity() << object->GetAngularVelocity();
                clientService_->Send(sendPacket);
            }
        }
    }
}

void GameScene::HandleEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Handle events
        if(event.type == sf::Event::Closed)
        {
            window.close();
        }
        if(gameStarted_)
        {
            if(drawChat_)
            {
                textInput_.HandleEvent(event, window);
            }
            else if(event.type == sf::Event::KeyPressed)
            {
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
                    case sf::Keyboard::Tab:
                        drawPlayerList_ = true;
                        break;
                    case sf::Keyboard::Enter:
                        drawChat_ = true;
                        textInput_.SetSelected(true);
                        break;
                    default:
                        break;
                }
            }
            // No else if here because important for exiting chat for example
            if(event.type == sf::Event::KeyReleased)
            {
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
                    case sf::Keyboard::Tab:
                        drawPlayerList_ = false;
                        break;
                    case sf::Keyboard::Escape:
                        drawChat_ = false;
                        textInput_.SetSelected(false);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void GameScene::Update(const sf::Time& deltaTime)
{
    game_->Update(deltaTime.asSeconds());
    if(gameStartClock_.getElapsedTime() > sf::seconds(10.0f) && !gameStarted_)
    {
        gameStarted_ = true;
        settings_->PlayTheme("gametheme");
    }
    if(currentLap_ == settings_->GetLaps() && !lastLapThemePlaying_ && gameStarted_)
    {
        settings_->PlayTheme("lastlaptheme");
        lastLapThemePlaying_ = true;
    } 
    //std::cout << "box2dcarpos: x " << game_->GetPlayerCar()->GetTransform().p.x << " y: " << game_->GetPlayerCar()->GetTransform().p.y << "sfmlpos x " << game_->GetPlayerCar()->GetSprite().getPosition().x << " y " << game_->GetPlayerCar()->GetSprite().getPosition().y << std::endl;
    // This line prints the friction under the car
    // std::cout << game_->GetMap()->GetFriction(game_->GetPlayerCar()->GetTransform().p) << std::endl;
}
void GameScene::Draw(sf::RenderWindow& window)
{
    if(playerFinished_)
    {
        // Draw whole map if player finished
        sf::View spectateView(sf::FloatRect(0.0f, settings_->GetVideoMode().height - game_->GetMap()->GetHeight() * game_->GetMap()->GetTileSize(), game_->GetMap()->GetWidth() * game_->GetMap()->GetTileSize(), game_->GetMap()->GetHeight() * game_->GetMap()->GetTileSize()));
        window.setView(spectateView);
        // Draw map
        window.draw(game_->GetMap()->GetMapDrawable());
        // Draw dynamic objects
        std::vector<DynamicObject*> objects = game_->GetObjects();
        for(auto o: objects) {
            window.draw(o->GetSprite());
        }
        // Set default view back
        window.setView(window.getDefaultView());

        // Draw chat
        if(drawChat_)
        {
            chat_.Draw(window);
            textInput_.Draw(window);
        }
        // Draw playerlist
        if(drawPlayerList_)
        {
            playerList_.Draw(window);
        } 
    }
    else
    {
        //Camera positioning
        sf::View view(sf::FloatRect(0.f, 0.f, 80.f, 45.f));
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

        //Draw player car and tires
        for(auto o: game_->GetPlayerCar()->GetTires()) {
            window.draw(o->GetSprite());
        }
        window.draw(game_->GetPlayerCar()->GetSprite());

        // Draw minimap border
        window.setView(window.getDefaultView());
        sf::RectangleShape rectangle({window.getSize().x * 0.25f, window.getSize().y * 0.25f});
        rectangle.setPosition({window.getSize().x * 0.75f, 0.0f});
        rectangle.setOutlineThickness(5.0f);
        rectangle.setOutlineColor(sf::Color::Black);
        window.draw(rectangle);

        // Draw minimap
        sf::View minimapView(sf::FloatRect(0.0f, settings_->GetVideoMode().height - game_->GetMap()->GetHeight() * game_->GetMap()->GetTileSize(), game_->GetMap()->GetWidth() * game_->GetMap()->GetTileSize(), game_->GetMap()->GetHeight() * game_->GetMap()->GetTileSize()));
        minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f)); // TODO Stop using magic numbers
        window.setView(minimapView);

        // Draw map
        window.draw(game_->GetMap()->GetMapDrawable());

        // Draw dynamic objects
        for(auto o: objects) {
            window.draw(o->GetSprite());
        }

        //Draw player car and tires
        for(auto o: game_->GetPlayerCar()->GetTires()) {
            window.draw(o->GetSprite());
        }
        window.draw(game_->GetPlayerCar()->GetSprite());

        // Set default view back
        window.setView(window.getDefaultView());

        // Draw chat
        if(drawChat_)
        {
            chat_.Draw(window);
            textInput_.Draw(window);
        }
        // Draw playerlist
        if(drawPlayerList_)
        {
            playerList_.Draw(window);
        }   
    }
}

// This is called when the current scene is changed to this one
void GameScene::Init()
{
    chat_.Init();
    playerList_.Init();
    textInput_.Init();
    // THIS IS HERE SO PLAY NOW WORKS OTHERWISE NOT NEEDED
    if(!clientService_->IsConnected())
    {
        game_ = new Game(clientService_->GetId(), settings_, 3, "FORMULA", "../res/maps/test_map_file.json");
    }
    theme2_.play();
}

// This is called when the current scene is changed to another one from this
void GameScene::Reset()
{
    chat_.Reset();
    playerList_.Reset();
    textInput_.Reset();
    theme2_.stop();
    playerFinished_ = false;
    if(game_ != nullptr)
    {
        delete game_;
        game_ = nullptr;
    }
}