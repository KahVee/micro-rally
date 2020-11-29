#include "Settings.hpp"
#include "../lib/json.hpp"

using json = nlohmann::json;

void Settings::SetVolume(float volume)
{
    sf::Listener::setGlobalVolume(volume);
    volume_ = volume;
}

float Settings::GetVolume()
{
    return volume_;
}

void Settings::SetLaps(int laps)
{
    laps_ = laps;
}

int Settings::GetLaps()
{
    return laps_;
}

void Settings::SetName(const std::string& name)
{
    playerName_ = name;
}

std::string Settings::GetName()
{
    return playerName_;
}

const sf::VideoMode& Settings::GetVideoMode()
{
    return sf::VideoMode::getFullscreenModes().at(resolutionIndex_);
}

void Settings::SetResolutionIndex(int resolutionIndex)
{
    resolutionIndex_ = resolutionIndex;
}

int Settings::GetResolutionIndex()
{
    return resolutionIndex_;
}

const CarData& Settings::GetCarData(const std::string& carType)
{
    return cars_[carType];
}

void Settings::SetFullscreen(bool fullscreen)
{
    fullscreen_ = fullscreen;
}

bool Settings::GetFullscreen()
{
    return fullscreen_;
}

bool Settings::LoadSettings()
{
    try
    {
        // Load settings
        std::ifstream file("../config/settings.json");
        json j;
        if (!(file.peek() == std::ifstream::traits_type::eof()))
        {
            file >> j;
            if(j.contains("PlayerName"))
            {
                playerName_ = j["PlayerName"].get<std::string>();
            }
            if(j.contains("Volume"))
            {
                volume_ =j["Volume"].get<float>();
            }
            if(j.contains("ResolutionIndex"))
            {
                resolutionIndex_ = j["ResolutionIndex"].get<int>();
            }
            if(j.contains("Fullscreen"))
            {
                fullscreen_ = j["Fullscreen"].get<bool>();
            }
        }
        sf::Listener::setGlobalVolume(volume_);
        // Load cars
        std::ifstream carFile("../res/cars.json");
        json jsonCars;
        carFile >> jsonCars;
        for(auto& element : jsonCars.items())
        {
            cars_[element.key()] = {
                element.value()["spritePath"].get<std::string>(),
                element.value()["tireStringPath"].get<std::string>(),
                element.value()["bodyWidth"].get<int>(),
                element.value()["bodyHeight"].get<int>(),
                element.value()["tirePositions"].get<std::vector<std::pair<float,float>>>(),
                element.value()["enginePower"].get<float>(),
                element.value()["brakingPower"].get<float>(),
                element.value()["maxSpeed"].get<float>(),
                element.value()["reverseSpeed"].get<float>(),
                element.value()["tireLockAngle"].get<float>(),
                element.value()["tireTurnSpeed"].get<float>(),
                element.value()["bodyDensity"].get<float>(),
            };
        }
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
        j["Volume"]  = volume_;
        j["ResolutionIndex"] = resolutionIndex_;
        j["Fullscreen"] = fullscreen_;
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