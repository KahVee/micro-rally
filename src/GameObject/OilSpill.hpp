#include "DynamicObject.hpp"

class Oilspill : public DynamicObject
{
public:
    Oilspill(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window);
    ~Oilspill();

private:
    void PrivateUpdate(float dt);
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};