#include "Settings.hpp"
#include "../lib/json.hpp"

using json = nlohmann::json;

Settings::~Settings()
{
    for(auto theme : themes_)
    {
        delete theme.second;
    }
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

const std::vector<std::string> Settings::GetCarNames()
{
    std::vector<std::string> carNames;
    for(auto& car : cars_)
    {
        carNames.push_back(car.first);
    }
    return carNames;
}

const std::vector<std::string>& Settings::GetMapNames()
{
    return maps_;
}

void Settings::SetCarIndex(int carIndex)
{
    carIndex_ = carIndex;
}

int Settings::GetCarIndex()
{
    return carIndex_;
}

void Settings::SetMapIndex(int mapIndex)
{
    mapIndex_ = mapIndex;
}

int Settings::GetMapIndex()
{
    return mapIndex_;
}

void Settings::SetFullscreen(bool fullscreen)
{
    fullscreen_ = fullscreen;
}

bool Settings::GetFullscreen()
{
    return fullscreen_;
}

void Settings::PlayTheme(const std::string& theme)
{
    StopTheme();
    themes_[theme]->play();
}
void Settings::StopTheme()
{
    for(auto theme : themes_)
    {
        theme.second->stop();
    }
}

void Settings::PlaySound(const std::string& sound)
{
    sounds_[sound].play();
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
        // Load maps
        maps_.push_back("test_map_file");
        maps_.push_back("test_map_file_2");
        // Load themes
        themes_["menutheme"] = new sf::Music;
        themes_["lastlaptheme"] = new sf::Music;
        themes_["scoreboardtheme"] = new sf::Music;
        themes_["gametheme"] = new sf::Music;
        themes_["gamestarttheme"] = new sf::Music;
        if(!themes_["menutheme"]->openFromFile("../res/audio/menutheme.wav")
        || !themes_["lastlaptheme"]->openFromFile("../res/audio/lastlaptheme.wav")
        || !themes_["scoreboardtheme"]->openFromFile("../res/audio/scoreboardtheme.wav")
        || !themes_["gametheme"]->openFromFile("../res/audio/gametheme.wav")
        || !themes_["gamestarttheme"]->openFromFile("../res/audio/gamestarttheme.wav"))
        {
            return false;
        }
        themes_["menutheme"]->setLoop(true);
        themes_["lastlaptheme"]->setLoop(true);
        themes_["scoreboardtheme"]->setLoop(true);
        themes_["gametheme"]->setLoop(true);
        themes_["gamestarttheme"]->setLoop(false);

        themes_["menutheme"]->setVolume(50.f);
        themes_["lastlaptheme"]->setVolume(50.f);
        themes_["scoreboardtheme"]->setVolume(50.f);
        themes_["gametheme"]->setVolume(50.f);
        themes_["gamestarttheme"]->setVolume(50.f);
        // Load sounds
        sf::SoundBuffer collisionsoundSoundBuffer;
        if(!collisionsoundSoundBuffer.loadFromFile("../res/audio/collisionsound.wav"))
        {
            return false;
        }
        soundBuffers_.push_back(collisionsoundSoundBuffer);
        sf::Sound collisionsound;
        collisionsound.setBuffer(soundBuffers_[soundBuffers_.size()-1]);
        sounds_["collisionsound"] = collisionsound;
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