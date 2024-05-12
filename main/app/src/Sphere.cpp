#include "Sphere.hpp"

Sphere::Sphere(float radius, float inverseMass) : SceneObject() {
	mesh = new SphereMesh(20, 20, radius);
	collider = new SphereCollider(radius);
	physicsBody = new PhysicsBody(inverseMass, 0.5f);
}

void Sphere::updateTransformations() {
	glm::vec3 v = physicsBody->getVelocity() * Time::deltaTime;
	mesh->getTransformation().translate(v);
	collider->translate(v);

	glm::vec3 o = physicsBody->getOrientation();
	glm::vec3 axis;
	float angle = glm::length(o);

	if (angle < 0.01f)
		angle = 0.0f;
	else
		mesh->getTransformation().setRotationAxis(glm::normalize(o));

	mesh->getTransformation().rotate(angle);
	collider->rotate(axis, angle);
}

void Sphere::updateInteractions(const std::vector<SceneObject*>& sceneObjects) {
	std::vector<Collider::CollisionData*> collisions;

	if (dragged)
		return;

	for (SceneObject* sceneObject : sceneObjects) {
		if (sceneObject != this) {
			auto collision = collider->collidesWith(*sceneObject->getCollider());
			if (collision)
				collisions.push_back(collision);
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
