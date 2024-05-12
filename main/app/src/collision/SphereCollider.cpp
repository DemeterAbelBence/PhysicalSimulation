#include "SphereCollider.hpp"

SphereCollider::SphereCollider(float _radius) {
	radius = _radius;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	type = Type::C_SPHERE;
}

Collider::CollisionData* SphereCollider::collidesWith(const CuboidCollider& collider) const {
	glm::vec3 colliderCenter = collider.calculateCenter();

	float dx = glm::abs(position.x - colliderCenter.x);
	float dy = glm::abs(position.y - colliderCenter.y);
	float dz = glm::abs(position.z - colliderCenter.z);

	float w = collider.getWidth() / 2.0f + radius;
	float h = collider.getHeight() / 2.0f + radius;
	float l = collider.getLength() / 2.0f + radius;

	if (dx <= w && dy <= h && dz <= l) {
		CuboidCollider::Side side = collider.getClosestSide(position, false);
		glm::vec3 n = side.normal;
		glm::vec3 v = -n;
		glm::vec3 p = position + radius * v;
		float d = collider.distanceFromSide(side, p);

		return new CollisionData(p, n, d);
	}
	else {
		return nullptr;
	}
}

Collider::CollisionData* SphereCollider::collidesWith(const SphereCollider& collider) const {
	float r = radius + collider.getRadius();
	glm::vec3 v = collider.getPosition() - position;
	float d = r - glm::length(v);

	if(d < 0.0f)
		return nullptr;
		
	glm::vec3 n = -normalize(v);
	glm::vec3 p = position + radius * n;
	return new CollisionData(p, n, d);
}

void SphereCollider::translate(glm::vec3 t) {
	transformation.translate(t);
	position += t;
}

void SphereCollider::scale(float s) {
	radius *= s;
}

void SphereCollider::rotate(glm::vec3 axis, float angle) {
}
