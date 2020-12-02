#pragma once

#include <vector>
#include <SFML/Network.hpp>

struct RaceState {
    int completedLaps;
    sf::Int32 nextRaceLineId;
};