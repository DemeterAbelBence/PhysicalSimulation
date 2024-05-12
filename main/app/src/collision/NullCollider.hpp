#pragma once

#include "Collider.hpp"
#include "CuboidCollider.hpp"

class NullCollider : public Collider {

public:
	NullCollider();

	Collider::CollisionData* collidesWith(const CuboidCollider& collider) const override;
	Collider::CollisionData* collidesWith(const SphereCollider& collider) const override;

	void translate(glm::vec3 t) override;
	void scale(float s) override;
	void rotate(glm::vec3 axis, float angle) override;
};