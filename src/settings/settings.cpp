#include "settings.hpp"

int settings::setName(std::string name)
{
    PlayerName = name;
    std::string turha;
    std::ofstream file;
    file.open("settings.json");
    file << "PlayerName:" << name << std::endl;
    file.close();
    return 0;
}

std::string settings::getName()
{
    std::string name = "player";
    std::string turha = "a";
    std::ifstream file;
    int a = 0;
    file.open("settings.json");
    while (!file.eof() && turha != "PlayerName" && a < 100)
    {
        a++;
        std::getline(file, turha, ':');
        if (turha == "PlayerName")
        {
            std::getline(file, name, '\n');
        }
    }   
    file.close();
    return name;
}