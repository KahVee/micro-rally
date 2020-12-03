#include "DynamicObject.hpp"

class TireStack : public DynamicObject
{
public:
    TireStack(sf::Int32 id, std::string spritePath, b2World *world, Settings* settings);
    ~TireStack();

private:
    void PrivateUpdate(float dt);
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};