#ifndef MANA_RIGIDBODYBT3_HPP
#define MANA_RIGIDBODYBT3_HPP

#include "platform/physics/3d/rigidbody3d.hpp"

namespace engine {
    class RigidBodyBt3 : public RigidBody3D {
    public:
        void setRigidBodyType(RigidBodyType type) override;

        RigidBodyType getRigidBodyType() override;

        void setPosition(const Vec3f &position) override;

        Vec3f getPosition() override;

        void setVelocity(const Vec3f &velocity) override;

        Vec3f getVelocity() override;

        void setRotation(const Vec3f &rotation) override;

        Vec3f getRotation() override;

        void setAngularVelocity(const Vec3f &angularVelocity) override;

        Vec3f getAngularVelocity() override;

        Collider3D *createCollider() override;

        void destroyCollider(Collider3D *collider) override;

        std::set<Collider3D *> getColliders() override;
    };
}

#endif //MANA_RIGIDBODYBT3_HPP
