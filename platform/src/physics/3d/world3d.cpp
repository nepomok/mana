#include "platform/physics/3d/world3d.hpp"

#include "../bullet3/worldbt3.hpp"

#include <stdexcept>

std::unique_ptr<engine::World3D> engine::World3D::create(engine::World3D::Backend backend) {
    switch (backend) {
        case BULLET3:
            return std::make_unique<engine::WorldBt3>();
    }
    throw std::runtime_error("Invalid 3d physics backend " + std::to_string(backend));
}
