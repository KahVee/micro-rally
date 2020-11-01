#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "Car.hpp"

Car::Car(b2World *world, int nOfTires): DynamicObject(sf::Sprite(), world) {
    enginePower_ = 1;
    airDrag_ = 1;

    for (int i = 0; i < nOfTires; i++) {
        Tire tire = Tire(sf::Sprite(), world);
        tires_.push_back(tire);
    }    
}

float Car::GetEnginePower() const {
    return enginePower_;
}

void Car::SetEnginePower(float newPower) {
    enginePower_ = newPower;
}

float Car::GetAirDrag() const {
    return airDrag_;
}

void Car::SetAirDrag(float newDrag) {
    airDrag_ = newDrag;
}

std::vector<Tire> Car::GetTires() {
    return tires_;
}
