#pragma once

#include "Collider.hpp"
#include "CuboidCollider.hpp"

class SphereCollider : public Collider {

private:
	glm::vec3 position;
	float radius;

public:
	SphereCollider(float _radius);

	inline glm::vec3 getPosition() const { return position; }
	inline float getRadius() const { return radius; }

	Collider::CollisionData* collidesWith(const CuboidCollider& collider) const override;
	Collider::CollisionData* collidesWith(const SphereCollider& collider) const override;

	void translate(glm::vec3 t) override;
	void scale(float s) override;
	void rotate(glm::vec3 axis, float angle) override;
};