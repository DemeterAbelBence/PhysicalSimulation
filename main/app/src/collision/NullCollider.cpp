#include "NullCollider.hpp"

NullCollider::NullCollider() {
    type = C_NULL;
}

Collider::CollisionData* NullCollider::collidesWith(const CuboidCollider& collider) const {
    return nullptr;
}

Collider::CollisionData* NullCollider::collidesWith(const SphereCollider& collider) const {
    return nullptr;
}

void NullCollider::translate(glm::vec3 t) {}

void NullCollider::scale(float s) {}

void NullCollider::rotate(glm::vec3 axis, float angle) {}
