#pragma once

#include <Box2D/Box2D.h>
#include <iostream>
class Game;
#include "Game.hpp"
#include "GameObject.hpp"
#include "../settings/Settings.hpp"
#include "../constants.hpp"

class ContactListener : public b2ContactListener {
    public:
    ContactListener(Game *game, Settings* settings): game_(game), settings_(settings) {};
    virtual ~ContactListener() {};
    virtual void BeginContact(b2Contact *contact);
    virtual void EndContact(b2Contact *contact) {};

    private:
    Game *game_;
    Settings* settings_;
};