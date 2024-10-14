#include "NullCollider.hpp"

NullCollider::NullCollider() {
    type = C_NULL;
}

std::vector<Collider::ContactData> NullCollider::collidesWith(const CuboidCollider& collider) const {
    return std::vector<Collider::ContactData>();
}
