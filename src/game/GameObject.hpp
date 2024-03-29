#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <Box2D/Box2D.h>

/* 
 * All entities in the game world are derived from GameObject (tiles, cars, obstacles)
 * At most basic level, a game object has a transform (position and rotation) and a sprite.
 */
class GameObject {
public:
    GameObject(sf::Int32 id) : id_(id) {};

    virtual ~GameObject() = default;

    sf::Int32 GetID() const;

    sf::Sprite GetSprite() const;
    void SetSprite(sf::Sprite);

    virtual b2Transform GetTransform() const = 0;
    virtual void SetTransform(b2Vec2, float) = 0;

protected:
    const sf::Int32 id_;
    void LoadSprite(std::string file);
    sf::Texture texture_;
    sf::Sprite sprite_;
    b2Transform transform_;
};