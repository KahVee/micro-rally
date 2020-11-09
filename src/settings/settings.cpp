#include "settings.hpp"
#include "../lib/json.hpp"

using json = nlohmann::json;

int settings::setName(std::string name)
{
    json j;
    std::ofstream file("settings.json");
    PlayerName = name;
    j["PlayerName"]  = name;
    file << j << std::endl;
    file.close();
    return 0;
}

std::string settings::getName()
{
    std::string name = "player";
    std::ifstream file("settings.json");
    json j;
    if (!(file.peek() == std::ifstream::traits_type::eof()))
    {
        file >> j;
        if (j.find("PlayerName") != j.end())
        {
        name = j["PlayerName"].get<std::string>();  
        }
        file.close();
        PlayerName = name;
    }
    return name;
}

