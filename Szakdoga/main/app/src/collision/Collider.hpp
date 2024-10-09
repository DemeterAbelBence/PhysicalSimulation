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
	Type type = C_NULL;

public:
	struct ContactData {
		glm::vec3 point;
		glm::vec3 normal;
		glm::vec3 edgeDirA;
		glm::vec3 edgeDirB;

		bool vertexFace;
	};

protected:
	Transformation* transformation;

	virtual std::vector<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
	virtual std::vector<ContactData> collidesWith(const SphereCollider& collider) const = 0;

public:
	inline void setTransformation(Transformation* value) { transformation = value; }
	inline const Transformation* getTransformation() const { return transformation; }

	std::vector<ContactData> collidesWith(const Collider& collider) const;

	virtual void updateTransformations() = 0;
};