#include "GameScene.hpp"

#include <vector>

GameScene::~GameScene() {
    delete game_;
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
            }
        }
    }
}

void GameScene::Update(const sf::Time& deltaTime)
{
    game_->Update(deltaTime.asSeconds());
}
void GameScene::Draw(sf::RenderWindow& window)
{
    // Draw map and objects
    sf::View view(sf::FloatRect(0.f, 0.f, 640.f, 360.f));
    view.setCenter(game_->GetPlayerCar()->GetSprite().getPosition());
    //view.setRotation(game_->GetPlayerCar()->GetTransform().q.GetAngle() * -RAD_TO_DEG);
    window.setView(view);
    window.draw(game_->GetMapSprite());
    std::vector<DynamicObject*> objects = game_->GetObjects();
    for(auto o: objects) {
        window.draw(o->GetSprite());
    }
    // Draw minimap border
    window.setView(window.getDefaultView());
    sf::RectangleShape rectangle({window.getSize().x * 0.25f, window.getSize().y * 0.25f});
    rectangle.setPosition({window.getSize().x * 0.75f, 0.0f});
    rectangle.setOutlineThickness(5.0f);
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setOutlineColor(sf::Color::Black);
    window.draw(rectangle);
    // Draw minimap
    sf::View minimapView(sf::FloatRect(0.f, 0.f, game_->GetMapSprite().getGlobalBounds().width, game_->GetMapSprite().getGlobalBounds().height ));
    minimapView.setViewport(sf::FloatRect(0.75f, 0.f, 0.25f, 0.25f)); // TODO Stop using magic numbers
    window.setView(minimapView);
    window.draw(game_->GetMapSprite());
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
}

// This is called when the current scene is changed to another one from this
void GameScene::Reset()
{
    delete game_;
}