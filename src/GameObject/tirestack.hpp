#include "DynamicObject.hpp"

class Tirestack : public DynamicObject
{
public:
    Tirestack(sf::Int32 id, std::string spritePath, b2World *world);
    ~Tirestack();

private:
    void PrivateUpdate(float dt);
    b2CircleShape shape_;
    b2FixtureDef fDef_;
};