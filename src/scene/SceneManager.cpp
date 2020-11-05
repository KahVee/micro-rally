#include "SceneManager.hpp"

SceneManager::~SceneManager()
{
    for(auto it = scenes_.begin(), end = scenes_.end(); it != end; it++)
    {
        delete it->second;
    }
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