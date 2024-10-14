#pragma once

#include <glm.hpp>
#include <iostream>
#include <vector>
#include <array>

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
		std::array<glm::vec3, 2> edgeA;
		std::array<glm::vec3, 2> edgeB;

		bool vertexFace;
	};

protected:
	Transformation* transformation;
	
public:
	inline void setTransformation(Transformation* value) { transformation = value; }
	inline const Transformation* getTransformation() const { return transformation; }

	std::vector<ContactData> collidesWith(const Collider& collider) const;

	virtual void updateTransformations();
	virtual std::vector<ContactData> collidesWith(const CuboidCollider& collider) const = 0;
};