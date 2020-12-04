#include "MapTile.hpp"

const float MapTile::GetFriction() const {
    return friction_;
}

const float MapTile::GetRollingResistance() const {
    return rollingResistance_;
}

const std::string MapTile::GetName() const {
    return name_;
}
