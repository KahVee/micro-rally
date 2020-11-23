#include "GameObject.hpp"
#include <iostream>

sf::Sprite GameObject::GetSprite() const {
    return sprite_;
}

void GameObject::LoadSprite(std::string file) {
    if (!texture_.loadFromFile(file)) {
        std::cout << "Couldn't load " << file << std::endl;
    }
    sprite_ = sf::Sprite(texture_);
    sf::Vector2u size = texture_.getSize();
    //Sets the rotation axis to center of the sprite (needed for displaying box2d rotations)
    sprite_.setOrigin(size.x/2.0, size.y/2.0);
}