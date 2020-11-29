#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
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
    ~Settings() = default;
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
    void SetFullscreen(bool fullscreen);
    bool GetFullscreen();
    bool LoadSettings();
    bool SaveSettings();
private:
    std::map<std::string,CarData> cars_;
    std::string playerName_ = "player";
    float volume_ = 50.0f;
    int resolutionIndex_ = 0;
    bool fullscreen_ = false;
    int laps_ = 1;
};