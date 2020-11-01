#include "GameObject.hpp"

GameObject::~GameObject() {

}

sf::Sprite GameObject::GetSprite() const {
    return sprite_;
}

void GameObject::SetSprite(sf::Sprite s) {
    sprite_ = s;
}