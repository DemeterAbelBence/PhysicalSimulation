#include "Collider.hpp"
#include "CuboidCollider.hpp"
#include "SphereCollider.hpp"

Collider::CollisionData* Collider::collidesWith(const Collider& collider) const {
    if (collider.type == Type::C_CUBOID) {
        auto c = dynamic_cast<const CuboidCollider&>(collider);
        return collidesWith(c);
    }

    if (collider.type == Type::C_SPHERE) {
        auto c = dynamic_cast<const SphereCollider&>(collider);
        return collidesWith(c);
    }

    return nullptr;
}
