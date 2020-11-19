# include "DynamicObject.hpp"

class Box : public DynamicObject
{
public:
    Box(sf::Int32 id, std::string spritePath, b2World *world);
    ~Box();
    
private:
    void PrivateUpdate(float dt);
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};