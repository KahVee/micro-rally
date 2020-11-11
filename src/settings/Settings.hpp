#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <SFML/Audio.hpp>

class Settings
{
public:
    Settings() = default;
    ~Settings() = default;
    void SetResolution(int width, int height);
    int GetHeight();
    int GetWidth();
    void SetVolume(float volume);
    float GetVolume();
    void SetName(const std::string& name);
    std::string GetName();
    bool LoadSettings();
    bool SaveSettings();
private:
    std::string playerName_;
    float volume_;
    int width_;
    int height_;
};