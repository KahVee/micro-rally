#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

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
    sf::VideoMode GetVideoMode();
    void SetResolutionIndex(int resolutionIndex);
    int GetResolutionIndex();
    void SetFullscreen(bool fullscreen);
    bool GetFullscreen();
    bool LoadSettings();
    bool SaveSettings();
private:
    sf::VideoMode videoMode_;
    std::string playerName_ = "player";
    float volume_ = 50.0f;
    int resolutionIndex_ = 0;
    bool fullscreen_ = false;
    int laps_ = 1;
};