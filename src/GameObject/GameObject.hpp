#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_body.h>

/* 
 * All entities in the game world are derived from GameObject (tiles, cars, obstacles)
 * At most basic level, a game object has a transform (position and rotation) and a sprite.
 */
class GameObject {
    public:
    virtual ~GameObject() = 0;

    sf::Sprite GetSprite() const;
    void SetSprite(sf::Sprite);

    virtual b2Transform GetTransform() const = 0;
    virtual void SetTransform(b2Vec2, float) = 0;

    protected:
    sf::Sprite sprite_;
    b2Transform transform_;
};