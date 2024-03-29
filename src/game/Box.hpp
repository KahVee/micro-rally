# include "DynamicObject.hpp"

class Box : public DynamicObject
{
public:
    Box(sf::Int32 id, std::string spritePath, b2World *world, sf::RenderWindow* window, float size);
    ~Box();
    
private:
    void PrivateUpdate(float dt);
    b2PolygonShape shape_;
    b2FixtureDef fDef_;
};