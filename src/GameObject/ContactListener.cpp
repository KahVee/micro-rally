#include <Box2D/Box2D.h>
#include <iostream>
#include "GameObject.hpp"

class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact *contact)
    {
        GameObject *fixtureAObj = static_cast<GameObject*>(contact->GetFixtureA()->GetUserData());
        GameObject *fixtureBObj = static_cast<GameObject*>(contact->GetFixtureB()->GetUserData());
        if (fixtureAObj != NULL && fixtureBObj != NULL) {
            std::cout << "Collision detected: ID: " << fixtureBObj->GetID() << ", ID2: " <<  fixtureAObj->GetID() << std::endl;
        }
    }

    void EndContact(b2Contact *contact)
    {
        
    }
};