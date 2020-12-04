#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <optional>
#include <exception>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../GameObject/CarData.hpp"

struct Resolution
{
    int width;
    int height;
};

class Settings
{
public:
    Settings() = default;
    ~Settings();
    void SetVolume(float volume);
    float GetVolume();
    void SetLaps(int laps);
    int GetLaps();
    void SetName(const std::string& name);
    std::string GetName();
    const sf::VideoMode& GetVideoMode();
    void SetResolutionIndex(int resolutionIndex);
    int GetResolutionIndex();
    const CarData& GetCarData(const std::string& carType);
    const std::vector<std::string> GetCarNames();
    const std::vector<std::string>& GetMapNames();
    void SetCarIndex(int carIndex);
    int GetCarIndex();
    void SetMapIndex(int mapIndex);
    int GetMapIndex();
    void SetFullscreen(bool fullscreen);
    bool GetFullscreen();
    void PlayTheme(const std::string& theme);
    void StopTheme();
    void PlaySound(const std::string& sound);
    bool LoadSettings();
    bool SaveSettings();
private:
    std::vector<sf::SoundBuffer> soundBuffers_;
    std::map<std::string,sf::Sound> sounds_;
    std::map<std::string,sf::Music*> themes_;
    std::map<std::string,CarData> cars_;
    std::vector<std::string> maps_;
    std::string playerName_ = "player";
    float volume_ = 50.0f;
    int resolutionIndex_ = 0;
    int carIndex_ = 0;
    int mapIndex_ = 0;
    bool fullscreen_ = false;
    int laps_ = 3;
};