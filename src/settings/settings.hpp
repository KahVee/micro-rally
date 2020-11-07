#include <string>
#include <fstream>
#include <iostream>

class settings
{
public:
    int setName(std::string Name);
    std::string getName();
private:
    std::string PlayerName;
}