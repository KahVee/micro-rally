#include "Settings.hpp"
#include "../lib/json.hpp"

using json = nlohmann::json;

void Settings::SetName(const std::string& name)
{
    playerName_ = name;
}

std::string Settings::GetName()
{
    return playerName_;
}


void Settings::SetResolution(int width, int height)
{
    width_ = width;
    height_ = height;
}

int Settings::GetWidth()
{
    return width_;
}

int Settings::GetHeight()
{
    return height_;
}

void Settings::SetVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
    volume_ = volume;
}

float Settings::GetVolume()
{
    return volume_;
}

bool Settings::LoadSettings()
{
    try
    {
        std::ifstream file("../config/settings.json");
        json j;
        file >> j;
        playerName_ = j["PlayerName"].get<std::string>();  
        width_ =j["Width"].get<int>();
        height_ =j["Height"].get<int>();
        volume_ =j["Volume"].get<float>();
        sf::Listener::setGlobalVolume(volume_);
        file.close();

    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool Settings::SaveSettings()
{
    try
    {
        json j;
        std::ofstream file("../config/settings.json");
        j["PlayerName"]  = playerName_;
        j["Width"]  = width_;
        j["Height"] = height_;
        j["Volume"]  = volume_;
        file << j << std::endl;
        file.close();

    }
    catch (const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
    return true;
}