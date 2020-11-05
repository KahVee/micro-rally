#include "MapTile.hpp"

const float MapTile::GetFriction() const {
    return friction_;
}

const std::string MapTile::GetName() const {
    return name_;
}
