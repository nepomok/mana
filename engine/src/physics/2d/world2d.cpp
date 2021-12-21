#include "engine/physics/2d/world2d.hpp"

#include "physics/box2d/worldbox2d.hpp"

#include <stdexcept>

std::unique_ptr<engine::World2D> engine::World2D::create(engine::World2D::Backend backend) {
    switch (backend) {
        case BOX2D:
            return std::make_unique<WorldBox2D>();
    }
    throw std::runtime_error("Invalid 2d physics backend " + std::to_string(backend));
}
