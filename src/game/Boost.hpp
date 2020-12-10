#include "DynamicObject.hpp"

class Boost : public DynamicObject
{
public:
    Boost(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window, float magnitude);
    ~Boost();

    float GetMagnitude();
    void SetMagnitude(float magnitude);
private:
    void PrivateUpdate(float dt);
    float magnitude_;
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};