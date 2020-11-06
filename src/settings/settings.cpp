#include <string>
#include <fstream>
#include <iostream>

class settings
{
public:

    int setName(std::string name)
    {
        PlayerName = name;
        std::string turha;
        std::ofstream file;
        file.open("settings.json");
        file << "PlayerName:" << name << std::endl;
        file.close();
        return 0;
    }

    std::string getName()
    {
        std::string name;
        std::string turha = "a";
        std::ifstream file;
        file.open("settings.json");
        while (!file.eof() && turha != "PlayerName")
        {
            std::getline(file, turha, ':');
            if (turha == "PlayerName")
            {
                std::getline(file, name, '\n');
            }
        }
        file.close();
        return name;
    }
private:
    std::string PlayerName;
};