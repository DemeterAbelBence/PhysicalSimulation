#pragma once

#include "Collider.hpp"
#include "CuboidCollider.hpp"

class NullCollider : public Collider {

public:
	NullCollider();

	std::vector<ContactData> collidesWith(const CuboidCollider& collider) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collider) const override;

	void updateTransformations() override;
};