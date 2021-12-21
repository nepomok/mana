#include "rigidbodybt3.hpp"

namespace engine {
    void RigidBodyBt3::setRigidBodyType(RigidBody3D::RigidBodyType type) {

    }

    RigidBody3D::RigidBodyType RigidBodyBt3::getRigidBodyType() {
        return DYNAMIC;
    }

    void RigidBodyBt3::setPosition(const Vec3f &position) {

    }

    Vec3f RigidBodyBt3::getPosition() {
        return Vec3f();
    }

    void RigidBodyBt3::setVelocity(const Vec3f &velocity) {

    }

    Vec3f RigidBodyBt3::getVelocity() {
        return Vec3f();
    }

    void RigidBodyBt3::setRotation(const Vec3f &rotation) {

    }

    Vec3f RigidBodyBt3::getRotation() {
        return Vec3f();
    }

    void RigidBodyBt3::setAngularVelocity(const Vec3f &angularVelocity) {

    }

    Vec3f RigidBodyBt3::getAngularVelocity() {
        return Vec3f();
    }

    Collider3D *RigidBodyBt3::createCollider() {
        return nullptr;
    }

    void RigidBodyBt3::destroyCollider(Collider3D *collider) {

    }

    std::set<Collider3D *> RigidBodyBt3::getColliders() {
        return std::set<Collider3D *>();
    }
}