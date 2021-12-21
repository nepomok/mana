#include "worldbt3.hpp"

namespace engine {
    RigidBody3D *WorldBt3::createRigidBody() {
        return nullptr;
    }

    void WorldBt3::destroyRigidBody(RigidBody3D *ptr) {

    }

    std::set<RigidBody3D *> WorldBt3::getRigidBodies() {
        return std::set<RigidBody3D *>();
    }

    Joint3D *WorldBt3::createJoint() {
        return nullptr;
    }

    void WorldBt3::destroyJoint(Joint3D *ptr) {

    }

    std::set<Joint3D *> WorldBt3::getJoints() {
        return std::set<Joint3D *>();
    }

    void WorldBt3::addContactListener(World3D::ContactListener *listener) {

    }

    void WorldBt3::removeContactListener(World3D::ContactListener *listener) {

    }

    void WorldBt3::setGravity(const Vec3f &gravity) {

    }

    void WorldBt3::step(float deltaTime) {

    }
}