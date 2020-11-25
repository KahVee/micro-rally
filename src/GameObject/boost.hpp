#include "DynamicObject.hpp"

class Boost : public DynamicObject
{
public:
    Boost(sf::Int32 id, std::string spritePath, b2World *world);
    ~Boost();

private:
    void PrivateUpdate(float dt);
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};