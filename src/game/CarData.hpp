#pragma once

#include <string>
#include <vector>

#include "../constants.hpp"

struct CarData {
    std::string spritePath;
    std::string tireStringPath;
    float bodyWidth;
    float bodyHeight;
    std::vector<std::pair<float, float>> tirePositions;
    float enginePower;
    float brakingPower;
    float maxSpeed;
    float reverseSpeed;
    float tireLockAngle;
    float tireTurnSpeed;
    float bodyDensity;
};