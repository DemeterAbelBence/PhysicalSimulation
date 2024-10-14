#include "Collider.hpp"
#include "CuboidCollider.hpp"

std::vector<Collider::ContactData> Collider::collidesWith(const Collider& collider) const {
    if (collider.type == Type::C_CUBOID) {
        auto c = dynamic_cast<const CuboidCollider&>(collider);
        return collidesWith(c);
    }

    return std::vector<Collider::ContactData>();
}

void Collider::updateTransformations() {}
