#pragma once

#include <array>
#include "Collider.hpp"
#include "SphereCollider.hpp"

class CuboidCollider : public Collider {

public:
	struct Side {
		std::array<glm::vec3, 4> points;
		glm::vec3 center;
		glm::vec3 normal;
	};

private:
	std::array<glm::vec3, 8> transPoints;
	std::array<Side, 6> transSides;

	std::array<glm::vec3, 8> basePoints;
	std::array<Side, 6> baseSides;

private:
	float width;
	float length;
	float height;

private:
	void createCorners();
	void createSides();
	Side createSide(const std::array<unsigned int, 4>& indices) const;

private:
	std::vector<glm::vec3> getEdgesOf(const CuboidCollider& collider) const;
	glm::vec3* calculateEdgeIntersection(glm::vec3 pa, glm::vec3 va, float a, glm::vec3 pb, glm::vec3 vb, float b) const;
	std::vector<ContactData> edgeEdgeCollision(const CuboidCollider& collider) const;

	bool calculateSideIntersection(glm::vec3 point, const Side& side) const;
	std::vector<ContactData> vertexFaceCollision(const CuboidCollider& collider) const;

public:
	CuboidCollider(float w, float h, float l);
	CuboidCollider(const CuboidCollider& c);

	inline float getWidth() const { return width; }
	inline float getLength() const { return length; }
	inline float getHeight() const { return height; }

	inline const std::array<glm::vec3, 8>& getTransPoints() const { return transPoints; }
	inline const std::array<glm::vec3, 8>& getBasePoints() const { return basePoints; }
	inline const std::array<Side, 6>& getTransSides() const { return transSides; }
	inline const std::array<Side, 6>& getBaseSides() const { return baseSides; }

	std::vector<glm::vec3> getSideDrawData(unsigned int sideIndex);

	std::vector<ContactData> collidesWith(const CuboidCollider& collider) const override;
	std::vector<ContactData> collidesWith(const SphereCollider& collider) const override;

	void updateTransformations() override;
};

