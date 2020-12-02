#include <Box2D/Box2D.h>
#include <iostream>
#include <SFML/Audio.hpp>
#include "GameObject.hpp"

class ContactListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact *contact)
    {
        GameObject *fixtureAObj = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
        GameObject *fixtureBObj = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());
        if (fixtureAObj != NULL && fixtureBObj != NULL) {
            std::cout << "Collision detected: ID: " << fixtureBObj->GetID() << ", ID2: " <<  fixtureAObj->GetID() << std::endl;
            settings_->PlaySound("collisionsound");       
        }
    }

    void EndContact(b2Contact *contact)
    {
        
    }
    
    void SetSettings(Settings* settings)
    {
        settings_ = settings;
    }
private:
    Settings* settings_ = nullptr;
};