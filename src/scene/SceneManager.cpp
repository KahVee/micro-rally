#include "SceneManager.hpp"

SceneManager::~SceneManager()
{
    for(auto it = scenes_.begin(), end = scenes_.end(); it != end; it++)
    {
        delete it->second;
    }
}

void SceneManager::Init(HostService& hostService, ClientService& clientService, std::thread& hostThread, Settings& settings, sf::RenderWindow& window, sf::Font& font, sf::Texture& menuBackgroundTexture, sf::SoundBuffer& buttonSoundBuffer)
{
    // Gray color
    sf::Color Gray(100,100,100,100);
    // Create main menu scene ------------------------------------------------------------------------------------------
    MenuScene* mainMenu = new MenuScene();
    mainMenu->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    mainMenu->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"2D CAR GAME", sf::Color::Red, font));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.32f}, {0.3f, 0.1f}, "", window,"PLAY NOW", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [this](){this->ChangeScene("game");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.44f}, {0.3f, 0.1f}, "", window,"HOST", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer,
        [&hostService, &hostThread, &clientService, this, &settings](){
            if(!hostService.IsRunning())
            {
                if(hostService.Listen(25000) == sf::Socket::Done)
                {
                    if(clientService.Connect("localhost", 25000, sf::seconds(5.0f), settings.GetName()) == sf::Socket::Done)
                    {
                        hostThread = std::thread(&HostService::Start, std::ref(hostService));
                        this->ChangeScene("hostlobby");
                    }
                    // TODO when client fails to connect
                }
            }
        }));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.56f}, {0.3f, 0.1f}, "", window,"JOIN", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [this](){this->ChangeScene("join");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.68f}, {0.3f, 0.1f}, "", window,"SETTINGS", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [this](){this->ChangeScene("settingsmenu");}));
    mainMenu->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, "", window,"QUIT", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [&window](){window.close();}));
    AddScene("mainMenu", mainMenu);

    // Create settingsmenu scene ------------------------------------------------------------------------------------------
    MenuScene* settingsmenu = new MenuScene();
    settingsmenu->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    settingsmenu->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"SETTINGS", sf::Color::Red, font));
    settingsmenu->AddSceneComponent(new ListSelectorSceneComponent({0.35f, 0.2f}, {0.3f, 0.1f}, "", window, sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, settings.GetFullscreen(), 2, 14,
        [&settings](int currentIndex){
            settings.SetFullscreen(currentIndex);
            if(currentIndex)
            {
                return "  FULLSCREEN  ";
            }
            else
            {
                return "NOT FULLSCREEN";
            }
        }));
    settingsmenu->AddSceneComponent(new ListSelectorSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, "", window, sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, settings.GetResolutionIndex(), sf::VideoMode::getFullscreenModes().size(), 12,
        [&settings](int currentIndex){
            settings.SetResolutionIndex(currentIndex);
            std::stringstream widthSs;
            widthSs << std::setw(4) << std::setfill(' ') << settings.GetVideoMode().width;

            std::stringstream heightSs;
            heightSs << std::setw(4) << std::setfill(' ') << settings.GetVideoMode().height;

            std::stringstream bpsSs;
            bpsSs << std::setw(2) << std::setfill(' ') << settings.GetVideoMode().bitsPerPixel;

            std::stringstream ss;
            ss << widthSs.str() << "-" << heightSs.str() << "-" << bpsSs.str();
            return ss.str();
        }));
    settingsmenu->AddSceneComponent(new TextInputSceneComponent({0.35f, 0.6f}, {0.3f, 0.1f}, "deselectonsubmit", window, settings.GetName(), sf::Color::Black, font, Gray, sf::Color::White, PLAYER_NAME_MAX_CHARACTERS,
        [&settings](const std::string& text){
            settings.SetName(text);
            return text;
        }));
    settingsmenu->AddSceneComponent(new SliderSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, "", window,"Volume: ", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, sf::Color::Black, settings.GetVolume()/100,
        [&settings](float relativeButtonPosition){
            float volume = relativeButtonPosition * 100.0f;
            settings.SetVolume(volume);
            std::stringstream ss;
            ss << std::setw(3) << std::setfill(' ') << static_cast<int>(volume);
            return ss.str();
        }));
    settingsmenu->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, "", window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [this](){this->ChangeScene("mainMenu");}));
    AddScene("settingsmenu", settingsmenu);
    // Create hostlobby scene ------------------------------------------------------------------------------------------
    MenuScene* hostlobby = new MenuScene();
    hostlobby->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    hostlobby->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"LOBBY", sf::Color::Red, font));
    hostlobby->AddSceneComponent(new TableSceneComponent({0.05f, 0.2f}, {0.4f, 0.6f}, "chat", window, sf::Color::Black, font, Gray, 15, {PLAYER_NAME_MAX_CHARACTERS, 20}));
    hostlobby->AddSceneComponent(new TableSceneComponent({0.5f, 0.2f}, {0.2f, 0.6f}, "playerlist", window, sf::Color::Black, font, Gray, MAX_CLIENTS, {3, PLAYER_NAME_MAX_CHARACTERS}));
    hostlobby->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.4f, 0.1f}, "", window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, &settings, &window](const std::string& text){
            if(clientService.IsConnected())
            {
                if(text.size() > 0 && text.at(0) == '/')
                {
                    if(text == "/ping")
                    {
                        // /ping command for debugging
                        sf::Packet packet;
                        packet << PING;
                        clientService.Send(packet);
                    }
                    else if(text == "/quit")
                    {
                        window.close();
                    }
                }
                else
                {
                    sf::Packet packet;
                    packet << CHAT_MESSAGE << settings.GetName() << text;
                    clientService.Send(packet);
                }
            }
            return "";
        }));
    hostlobby->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, "", window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer,
        [&hostService, &hostThread, &clientService](){
            hostService.Stop();
            // Terminate thread if needed
            if(hostThread.joinable())
            {
                hostThread.join();
            }
            clientService.Disconnect();
        }));
    hostlobby->AddSceneComponent(new ButtonSceneComponent({0.5f, 0.85f}, {0.2f, 0.1f}, "", window,"PLAY", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer,
        [&clientService, &settings](){
            // Send start message to the hostService
            sf::Packet packet;
            sf::Int32 map = settings.GetMapIndex();
            sf::Int32 laps = settings.GetLaps();
            packet << GAME_START << map << laps;
            clientService.Send(packet);
        }));
    hostlobby->AddSceneComponent(new SliderSceneComponent({0.75f, 0.2f}, {0.2f, 0.1f}, "", window,"Laps: ", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, sf::Color::Black, (settings.GetLaps()-1)/(MAX_LAPS-1),
        [&settings](float relativeButtonPosition){
            int laps = static_cast<int>(relativeButtonPosition * (MAX_LAPS-1) + 1);
            settings.SetLaps(laps);
            std::stringstream ss;
            ss << std::setw(3) << std::setfill(' ') << static_cast<int>(laps);
            return ss.str();
        }));
    hostlobby->AddSceneComponent(new ListSelectorSceneComponent({0.75f, 0.4f}, {0.2f, 0.1f}, "", window, sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, settings.GetMapIndex(), settings.GetMapNames().size(), 16,
        [&settings](int currentIndex){
            settings.SetMapIndex(currentIndex);
            return settings.GetMapNames()[currentIndex];
        }));
    hostlobby->AddSceneComponent(new ListSelectorSceneComponent({0.75f, 0.6f}, {0.2f, 0.1f}, "", window, sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, settings.GetCarIndex(), settings.GetCarNames().size(), 10,
        [&settings, &clientService](int currentIndex){
            settings.SetCarIndex(currentIndex);
            if(clientService.IsConnected())
            {
                sf::Packet packet;
                sf::Int32 carIndex = currentIndex;
                packet << CLIENT_CAR << carIndex;
                clientService.Send(packet);
            }
            return settings.GetCarNames()[currentIndex];
        }));
    AddScene("hostlobby", hostlobby);
    // Create join scene ------------------------------------------------------------------------------------------
    MenuScene* join = new MenuScene();
    join->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    join->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"JOIN", sf::Color::Red, font));
    join->AddSceneComponent(new TextInputSceneComponent({0.35f, 0.4f}, {0.3f, 0.1f}, "deselectonsubmit", window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, this, &settings](const std::string& text){
            if(!clientService.IsConnected())
            {
                if(clientService.Connect(text, 25000, sf::seconds(5.0f), settings.GetName()) == sf::Socket::Done)
                {
                    this->ChangeScene("clientlobby");
                }
            }
            return text;
        }));
    join->AddSceneComponent(new ButtonSceneComponent({0.35f, 0.8f}, {0.3f, 0.1f}, "", window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, [this](){this->ChangeScene("mainMenu");}));
    AddScene("join", join);
    // Create clientlobby scene ------------------------------------------------------------------------------------------
    MenuScene* clientlobby = new MenuScene();
    clientlobby->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    clientlobby->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"LOBBY", sf::Color::Red, font));
    clientlobby->AddSceneComponent(new TableSceneComponent({0.05f, 0.2f}, {0.4f, 0.6f}, "chat", window, sf::Color::Black, font, Gray, 15, {PLAYER_NAME_MAX_CHARACTERS, 20}));
    clientlobby->AddSceneComponent(new TableSceneComponent({0.5f, 0.2f}, {0.2f, 0.6f}, "playerlist", window, sf::Color::Black, font, Gray, MAX_CLIENTS, {3, PLAYER_NAME_MAX_CHARACTERS}));
    clientlobby->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.4f, 0.1f}, "", window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, &settings, &window](const std::string& text){
            if(clientService.IsConnected())
            {
                if(text.size() > 0 && text.at(0) == '/')
                {
                    if(text == "/ping")
                    {
                        // /ping command for debugging
                        sf::Packet packet;
                        packet << PING;
                        clientService.Send(packet);
                    }
                    else if(text == "/quit")
                    {
                        window.close();
                    }
                }
                else
                {
                    sf::Packet packet;
                    packet << CHAT_MESSAGE << settings.GetName() << text;
                    clientService.Send(packet);
                }
            }
            return "";
        }));
    clientlobby->AddSceneComponent(new ListSelectorSceneComponent({0.75f, 0.6f}, {0.2f, 0.1f}, "", window, sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer, settings.GetCarIndex(), settings.GetCarNames().size(), 10,
        [&settings, &clientService](int currentIndex){
            settings.SetCarIndex(currentIndex);
            if(clientService.IsConnected())
            {
                sf::Packet packet;
                sf::Int32 carIndex = currentIndex;
                packet << CLIENT_CAR << carIndex;
                clientService.Send(packet);
            }
            return settings.GetCarNames()[currentIndex];
        }));
    clientlobby->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, "", window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer,
        [&clientService](){
            clientService.Disconnect();
        }));
    AddScene("clientlobby", clientlobby);
    // Create scorescreen scene ------------------------------------------------------------------------------------------
    MenuScene* scorescreen = new MenuScene();
    scorescreen->AddSceneComponent(new PictureSceneComponent({0.0f, 0.0f}, {1.0f, 1.0f}, "", window, menuBackgroundTexture));
    scorescreen->AddSceneComponent(new TextSceneComponent({0.3f, 0.0f}, {0.4f, 0.2f}, "", window,"SCORE SCREEN", sf::Color::Red, font));
    scorescreen->AddSceneComponent(new TableSceneComponent({0.5f, 0.2f}, {0.4f, 0.6f}, "scorelist", window, sf::Color::Black, font, Gray, MAX_CLIENTS, {3, PLAYER_NAME_MAX_CHARACTERS, 3}));
    scorescreen->AddSceneComponent(new TableSceneComponent({0.05f, 0.2f}, {0.4f, 0.6f}, "chat", window, sf::Color::Black, font, Gray, 15, {PLAYER_NAME_MAX_CHARACTERS, 20}));
    scorescreen->AddSceneComponent(new TextInputSceneComponent({0.05f, 0.85f}, {0.4f, 0.1f}, "", window,"", sf::Color::Black, font, Gray, sf::Color::White, 20,
        [&clientService, &settings, &window](const std::string& text){
            if(clientService.IsConnected())
            {
                if(text.size() > 0 && text.at(0) == '/')
                {
                    if(text == "/ping")
                    {
                        // /ping command for debugging
                        sf::Packet packet;
                        packet << PING;
                        clientService.Send(packet);
                    }
                    else if(text == "/quit")
                    {
                        window.close();
                    }
                }
                else
                {
                    sf::Packet packet;
                    packet << CHAT_MESSAGE << settings.GetName() << text;
                    clientService.Send(packet);
                }
            }
            return "";
        }));
    scorescreen->AddSceneComponent(new ButtonSceneComponent({0.05f, 0.05f}, {0.2f, 0.1f}, "", window,"BACK", sf::Color::Black, font, Gray, sf::Color::White, buttonSoundBuffer,
        [&clientService, &hostService, &hostThread](){
            hostService.Stop();
            // Terminate thread if needed
            if(hostThread.joinable())
            {
                hostThread.join();
            }
            clientService.Disconnect();
        }));
    AddScene("scorescreen", scorescreen);
    // Create game scene ------------------------------------------------------------------------------------------
    GameScene* game = new GameScene(&clientService, window, font, settings, Gray);
    AddScene("game", game);
    // Set initial scene
    SetInitialScene("mainMenu");
}

void SceneManager::AddScene(const std::string& sceneName, Scene* scenePointer)
{
    scenes_[sceneName] = scenePointer;
}

void SceneManager::SetInitialScene(const std::string& sceneName)
{
    currentScene_ = scenes_[sceneName];
    currentScene_->Init();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
    currentScene_->Reset();
    currentScene_ = scenes_[sceneName];
    currentScene_->Init();
}

Scene* SceneManager::GetCurrentScene() const
{
    return currentScene_;
}

void SceneManager::HandlePacket(sf::Packet& packet)
{
    currentScene_->HandlePacket(packet);
}

void SceneManager::HandleEvents(sf::RenderWindow& window)
{
    currentScene_->HandleEvents(window);
}

void SceneManager::Update(const sf::Time& deltaTime)
{
    currentScene_->Update(deltaTime);
}

void SceneManager::Draw(sf::RenderWindow& window)
{
    currentScene_->Draw(window);
}