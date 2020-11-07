#include <SFML/Network.hpp>

struct Client
{
    sf::Int32 id;
    sf::TcpSocket* socket;
    std::string name;
};