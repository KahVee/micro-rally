#include "DynamicObject.hpp"

class TireStack : public DynamicObject
{
public:
    TireStack(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window);
    ~TireStack();

private:
    void PrivateUpdate(float dt);
    b2CircleShape shape_;
    b2FixtureDef fDef_;
};