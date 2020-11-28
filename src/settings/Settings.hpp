#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <exception>
#include <SFML/Audio.hpp>

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
    int GetHeight();
    int GetWidth();
    void SetVolume(float volume);
    float GetVolume();
    void SetResolutionIndex(int resolutionIndex);
    int GetResolutionIndex();
    void SetLaps(int laps);
    int GetLaps();
    void SetName(const std::string& name);
    std::string GetName();
    bool LoadSettings();
    bool SaveSettings();
    static const std::vector<Resolution> resolutions;
private:
    std::string playerName_ = "player";
    float volume_ = 50.0f;
    int resolutionIndex_ = 0;
    int laps_ = 1;
};