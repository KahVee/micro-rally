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

const sf::VideoMode& Settings::GetFullscreenVideoMode()
{
    return sf::VideoMode::getFullscreenModes()[fullscreenResolutionIndex_];
}

const sf::VideoMode& Settings::GetWindowedVideoMode()
{
    return windowedModes[windowedResolutionIndex_];
}

void Settings::SetFullscreenResolutionIndex(int fullscreenResolutionIndex)
{
    fullscreenResolutionIndex_ = fullscreenResolutionIndex;
}

int Settings::GetFullscreenResolutionIndex()
{
    return fullscreenResolutionIndex_;
}

void Settings::SetWindowedResolutionIndex(int windowedResolutionIndex)
{
    windowedResolutionIndex_ = windowedResolutionIndex;
}

int Settings::GetWindowedResolutionIndex()
{
    return windowedResolutionIndex_;
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
            if(j.contains("FullscreenResolutionIndex"))
            {
                fullscreenResolutionIndex_ = j["FullscreenResolutionIndex"].get<int>();
            }
            if(j.contains("WindowedResolutionIndex"))
            {
                windowedResolutionIndex_ = j["WindowedResolutionIndex"].get<int>();
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
                element.value()["bodyWidth"].get<float>(),
                element.value()["bodyHeight"].get<float>(),
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
        std::ifstream mapFile("../res/maps.json");
        json jsonMaps;
        mapFile >> jsonMaps;
        for(auto& element : jsonMaps.items())
        {
            maps_.push_back(element.key());
        }
        // Load themes
        themes_["menutheme"] = new sf::Music;
        themes_["lastlaptheme"] = new sf::Music;
        themes_["scoreboardtheme"] = new sf::Music;
        themes_["gametheme"] = new sf::Music;
        themes_["countdowntheme"] = new sf::Music;
        if(!themes_["menutheme"]->openFromFile("../res/audio/8bit-boogiewoogie.wav")
        || !themes_["lastlaptheme"]->openFromFile("../res/audio/lastlaptheme.wav")
        || !themes_["scoreboardtheme"]->openFromFile("../res/audio/scoreboardtheme.wav")
        || !themes_["gametheme"]->openFromFile("../res/audio/gametheme.wav")
        || !themes_["countdowntheme"]->openFromFile("../res/audio/countdowntheme.wav"))
        {
            return false;
        }
        themes_["menutheme"]->setLoop(true);
        themes_["lastlaptheme"]->setLoop(true);
        themes_["scoreboardtheme"]->setLoop(true);
        themes_["gametheme"]->setLoop(true);
        themes_["countdowntheme"]->setLoop(false);

        themes_["lastlaptheme"]->setLoopPoints(sf::Music::Span<sf::Time>(sf::seconds(3), sf::seconds(54.250)));

        themes_["menutheme"]->setVolume(50.f);
        themes_["lastlaptheme"]->setVolume(50.f);
        themes_["scoreboardtheme"]->setVolume(50.f);
        themes_["gametheme"]->setVolume(50.f);
        themes_["countdowntheme"]->setVolume(50.f);
        // Load sounds
        sf::SoundBuffer collisionsoundSoundBuffer;
        sf::SoundBuffer buttonsoundSoundBuffer;
        sf::SoundBuffer checkpointsoundSoundBuffer;
        if(!collisionsoundSoundBuffer.loadFromFile("../res/audio/collisionsound.wav")
        || !buttonsoundSoundBuffer.loadFromFile("../res/audio/buttonsound.wav")
        || !checkpointsoundSoundBuffer.loadFromFile("../res/audio/checkpointsound.wav"))
        {
            return false;
        }
        soundBuffers_.push_back(collisionsoundSoundBuffer);
        soundBuffers_.push_back(buttonsoundSoundBuffer);
        soundBuffers_.push_back(checkpointsoundSoundBuffer);
        sf::Sound collisionsound;
        sf::Sound buttonsound;
        sf::Sound checkpointsound;
        collisionsound.setBuffer(soundBuffers_[0]);
        buttonsound.setBuffer(soundBuffers_[1]);
        checkpointsound.setBuffer(soundBuffers_[2]);
        sounds_["collisionsound"] = collisionsound;
        sounds_["buttonsound"] = buttonsound;
        sounds_["checkpointsound"] = checkpointsound;
        sounds_["collisionsound"].setAttenuation(0);
        sounds_["buttonsound"].setAttenuation(0);
        sounds_["checkpointsound"].setAttenuation(0);
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
        j["FullscreenResolutionIndex"] = fullscreenResolutionIndex_;
        j["WindowedResolutionIndex"] = windowedResolutionIndex_;
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

const std::vector<sf::VideoMode> Settings::windowedModes = {sf::VideoMode(854,480, sf::VideoMode::getDesktopMode().bitsPerPixel), 
                                                          sf::VideoMode(1280,720, sf::VideoMode::getDesktopMode().bitsPerPixel),
                                                          sf::VideoMode(1920,1080, sf::VideoMode::getDesktopMode().bitsPerPixel)};