#pragma once

#include <vector>
#include <SFML/Network.hpp>

struct RaceState {
    int currentLap;
    sf::Int32 nextRaceLineId;
};