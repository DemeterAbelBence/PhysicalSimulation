#include "Box.hpp"

Box::Box(float width, float height, float length, float inverseMass) : SceneObject() {
	mesh = new CuboidMesh(width, height, length);
	collider = new CuboidCollider(width, height, length);
	physicsBody = new PhysicsBody(inverseMass, 100.0f);
}

void Box::updateTransformations() {
	glm::vec3 v = physicsBody->getVelocity() * Time::deltaTime;
	mesh->getTransformation().translate(v);
	collider->translate(v);
}

void Box::updateInteractions(const std::vector<SceneObject*>& sceneObjects) {
	std::vector<Collider::CollisionData*> collisions;

	if (dragged)
		return;

	for (SceneObject* sceneObject : sceneObjects) {
		if (sceneObject != this) {
			auto collision = collider->collidesWith(*sceneObject->getCollider());
			if (collision) {
				collision = collider->collidesWith(*sceneObject->getCollider());
				collisions.push_back(collision);
			}
		}
	}

	for (Collider::CollisionData* c : collisions) {
		if (physicsBody->getInverseMass() > 0.0f) {
			glm::vec3 t = (c->collision_depth - 0.001f) * c->contactNormal;
			translate(t);
		}
	}

	physicsBody->exertForce(glm::vec3(0.0f, -1.0f, 0.0f), 5.0f);
	physicsBody->update(collisions);
}
