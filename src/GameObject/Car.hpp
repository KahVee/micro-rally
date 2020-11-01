#pragma once

#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>

#include "DynamicObject.hpp"
#include "Tire.hpp"

class Car : public DynamicObject {
    public:
    Car(b2World *world, int nOfTires);
    virtual ~Car() {}

    float GetEnginePower() const;
    void SetEnginePower(float newPower);
    float GetAirDrag() const;
    void SetAirDrag(float newDrag);

    std::vector<Tire> GetTires();

    private:
    float enginePower_;
    float airDrag_;
    std::vector<Tire> tires_;
};