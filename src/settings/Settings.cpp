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

int Settings::GetWidth()
{
    return resolutions[resolutionIndex_].width;
}

int Settings::GetHeight()
{
    return resolutions[resolutionIndex_].height;
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

void Settings::SetResolutionIndex(int resolutionIndex)
{
    resolutionIndex_ = resolutionIndex;
}

int Settings::GetResolutionIndex()
{
    return resolutionIndex_;
}

void Settings::SetLaps(int laps)
{
    laps_ = laps;
}

int Settings::GetLaps()
{
    return laps_;
}

bool Settings::LoadSettings()
{
    try
    {
        std::ifstream file("../config/settings.json");
        json j;
        if (!(file.peek() == std::ifstream::traits_type::eof()))
        {
            file >> j;
            if(j.contains("PlayerName"))
            {
                playerName_ = j["PlayerName"].get<std::string>();
            }
            if(j.contains("ResolutionIndex"))
            {
                resolutionIndex_ = j["ResolutionIndex"].get<int>();
            }
            if(j.contains("Volume"))
            {
                volume_ =j["Volume"].get<float>();
            }
        }
        sf::Listener::setGlobalVolume(volume_);
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
        j["ResolutionIndex"] = resolutionIndex_;
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

const std::vector<Resolution> Settings::resolutions = {{854, 480}, {1280, 720}, {1920, 1080}};