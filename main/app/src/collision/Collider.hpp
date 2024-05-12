#pragma once

#include <glm.hpp>
#include <iostream>
#include <vector>

#include "../util/Transformation.hpp"

class CuboidCollider;
class SphereCollider;

class Collider {
public:
	enum Type {
		C_SPHERE,
		C_CUBOID,
		C_NULL
	};
	Type type;

public:
	struct CollisionData {
		glm::vec3 contactPoint;
		glm::vec3 contactNormal;
		float collision_depth;

		CollisionData(glm::vec3 p, glm::vec3 n, float c) {
			contactPoint = p;
			contactNormal = n;
			collision_depth = c;
		}
	};

protected:
	Transformation transformation;

	virtual CollisionData* collidesWith(const CuboidCollider& collider) const = 0;
	virtual CollisionData* collidesWith(const SphereCollider& collider) const = 0;

public:
	CollisionData* collidesWith(const Collider& collider) const;

	virtual void translate(glm::vec3 t) = 0;
	virtual void scale(float s) = 0;
	virtual void rotate(glm::vec3 axis, float angle) = 0;
};