#include "GameObject/Tire.hpp"
#include "GameObject/Car.hpp"
#include "GameObject/DynamicObject.hpp"
#include "GameObject/GameObject.hpp"
#include <box2d/b2_world.h>
#include <box2d/b2_math.h>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>

int main() {
    b2Vec2 g = b2Vec2(0,0);
    b2World *world = new b2World(g);
    Tire tire = Tire(sf::Sprite(), world);

    std::cout << "(" << tire.GetTransform().p.x << ", " << tire.GetTransform().p.y << "), " << tire.GetTransform().q.GetAngle() << std::endl;
    tire.DebugApplyForce();
    for (int i = 0; i < 20; i++)
    {
        world->Step(0.05,3,8);
        tire.UpdateFriction();
        std::cout << "(" << tire.GetTransform().p.x << ", " << tire.GetTransform().p.y << "), " << tire.GetTransform().q.GetAngle() << std::endl;
    }

    delete world;
    return 0;
}