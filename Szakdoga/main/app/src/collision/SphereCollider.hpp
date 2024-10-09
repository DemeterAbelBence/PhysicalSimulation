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

	std::vector<ContactData> collidesWith(const CuboidCollider& collider) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collider) const override;

	void updateTransformations() override;
};