#pragma once

#include <array>
#include "Collider.hpp"
#include "SphereCollider.hpp"

class CuboidCollider : public Collider {

public:
	struct Side {
		glm::vec3 center;
		glm::vec3 normal;
	};

private:
	std::array<glm::vec3, 8> points;
	std::array<Side, 6> sides;

private:
	float width;
	float length;
	float height;

private:
	void createCorners();
	void createSides();
	Side createSide(const std::array<unsigned int, 4>& indices) const;

public:
	CuboidCollider(float w, float h, float l);
	CuboidCollider(const CuboidCollider& c);

	inline float getWidth() const { return width; }
	inline float getLength() const { return length; }
	inline float getHeight() const { return height; }
	inline const std::array<glm::vec3, 8>& getPoints() const { return points; }

	glm::vec3* calculateEdgeContact(const CuboidCollider& c1, const CuboidCollider& c2) const;
	glm::vec3 calculateCenter() const;
	CuboidCollider::Side getClosestSide(const glm::vec3& point, bool insideCuboid) const;
	float distanceFromSide(const Side& side, const glm::vec3& point) const;

	Collider::CollisionData* collidesWith(const CuboidCollider& collider) const override;
	Collider::CollisionData* collidesWith(const SphereCollider& collider) const override;

	void translate(glm::vec3 t) override;
	void scale(float s) override;
	void rotate(glm::vec3 axis, float angle) override;
};

