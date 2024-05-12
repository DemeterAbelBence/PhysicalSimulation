#pragma once

#include <vector>
#include <iostream>
#include <glm.hpp>

#include "../collision/Collider.hpp"
#include "../util/glfw/Time.hpp"

class PhysicsBody {
private:
	float inverseMass;
	float bounciness;

	glm::vec3 position;
	glm::vec3 resultant;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 orientation;
	
private:
	glm::vec3 calculateReflection(glm::vec3 vector, glm::vec3 normal);

public:
	PhysicsBody();
	PhysicsBody(float _inverseMass, float _bounciness);

	inline glm::vec3 getPosition() const { return position; }
	inline glm::vec3 getVelocity() const { return velocity; }
	inline glm::vec3 getAcceleration() const { return velocity; }
	inline glm::vec3 getOrientation() const { return orientation; }
	inline float getInverseMass() const { return inverseMass; }

	inline void setPosition(glm::vec3 value) { position = value; }
	inline void setVelocity(glm::vec3 value) { velocity = value; }
	inline void setAcceleration(glm::vec3 value) { acceleration = value; }

	void exertForce(glm::vec3 direction, float force);
	void update(std::vector<Collider::CollisionData*> collisions);

	void translate(glm::vec3 t);
	void rotate(glm::vec3 axis, float angle);
};