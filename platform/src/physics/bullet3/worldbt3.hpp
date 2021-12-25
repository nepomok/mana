#ifndef MANA_WORLDBT3_HPP
#define MANA_WORLDBT3_HPP


#include "../../../include/platform/physics/3d/world3d.hpp"

namespace engine {
    class WorldBt3 : public World3D {
    public:
        RigidBody3D *createRigidBody() override;

        void destroyRigidBody(RigidBody3D *ptr) override;

        std::set<RigidBody3D *> getRigidBodies() override;

        Joint3D *createJoint() override;

        void destroyJoint(Joint3D *ptr) override;

        std::set<Joint3D *> getJoints() override;

        void addContactListener(ContactListener *listener) override;

        void removeContactListener(ContactListener *listener) override;

        void setGravity(const Vec3f &gravity) override;

        void step(float deltaTime) override;
    };
}


#endif //MANA_WORLDBT3_HPP
