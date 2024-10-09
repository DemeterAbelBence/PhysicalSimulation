#include "SphereCollider.hpp"

SphereCollider::SphereCollider(float _radius) {
	radius = _radius;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	type = Type::C_SPHERE;
}

std::vector<Collider::ContactData> SphereCollider::collidesWith(const CuboidCollider& collider) const {
	return std::vector<ContactData>();
}

std::vector<Collider::ContactData> SphereCollider::collidesWith(const SphereCollider& collider) const {
	return std::vector<ContactData>();
}

void SphereCollider::updateTransformations() {}