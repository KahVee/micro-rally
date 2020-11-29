#pragma once
#include "GameObject/CarData.hpp"

static const float DEG_TO_RAD = 0.0174532925;
static const float RAD_TO_DEG = 57.295779513;
//static const int PIXELS_PER_METER = 10;
static const int MAP_TEXTURE_MAP_SIZE = 512;
static const int MAP_TEXTURE_TILES = 16;
static const int MAP_TEXTURE_TILE_SIZE = 32;
static const int RANDOM_SEED = 21;
// MAX LAPS CANT BE smaller than 2
static const int MAX_LAPS = 10;
static const int PLAYER_NAME_MAX_CHARACTERS = 10;

const CarData FORMULA {
    "../res/f1.png",
    "../res/tire.png",
    2, // width
    4, // length
    { {-0.8, 1.1 }, {0.8, 1.1}, {-0.8, -1.7}, {0.8, -1.7}}, // tire positions
    100, // engine power
    80, // braking/reverse power
    40, // top speed
    15, // reverse speed
    35 * DEG_TO_RAD, // max steering angle
    160, // steeering speed
    1.2 // body density
    };

    const CarData TRUCK {
    "../res/truck.png",
    "../res/tire.png",
    2, // width
    6, // length
    { {-0.9, 2.0}, {0.9, 2.0}, {-1.0, -2.0}, {1.0, -2.0}}, // tire positions
    150, // engine power
    60, // braking/reverse power
    30, // top speed
    10, // reverse speed
    20 * DEG_TO_RAD, // max steering angle
    120, // steering speed
    2 // body density
    };