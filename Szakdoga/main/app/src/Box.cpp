#include "Box.hpp"

#include <cstdlib> 
#include <ctime>   

void Box::rollbackContactWith(SceneObject* other) {
	float dt = delta_time / 10.0f;
	auto contactData = collider->collidesWith(*other->getCollider());
	while (contactData.size() > 0) {
		updateBodyState(-dt);
		updateTransformations();
		contactData = collider->collidesWith(*other->getCollider());
	};
	updateBodyState(2.0f * dt);
	updateTransformations();
}

Box::Box(float width, float height, float length, bool stationary) : SceneObject() {
	mesh = new CuboidMesh(width, height, length);
	collider = new CuboidCollider(width, height, length);

	float w = width;
	float h = height;
	float l = length;

	float inverseMass;
	glm::mat3 Ibody;
	glm::mat3 Ibodyinv;

	if (!stationary) {
		inverseMass = 0.01f;
		float angularMass = 1000.0f;

		Ibody = angularMass / 12.0f * glm::mat3(
			glm::vec3(h * h + l * l, 0.0f, 0.0f),
			glm::vec3(0.0f, w * w + l * l, 0.0f),
			glm::vec3(0.0f, 0.0f, w * w + h * h)
		);

		Ibodyinv = 12.0f / angularMass * glm::mat3(
			glm::vec3(1.0f / (h * h + l * l), 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f / (w * w + l * l), 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f / (w * w + h * h))
		);
	}
	else {
		inverseMass = 0.0f;
		Ibody = glm::mat3(0.0f);
		Ibodyinv = glm::mat3(0.0f);
	}

	auto bodyData = RigidBody::BodyData{
		inverseMass,			// invMass
		Ibody,                  // Ibody
		Ibodyinv,               // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P 
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f, -1.0f, 0.0f),        // force
		glm::vec3(0.0f)                      // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(bodyData);

	transformation = new Transformation();
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void Box::resetBodyState() {
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void Box::updateBodyState(float dt) {
	auto sides = dynamic_cast<CuboidCollider*>(collider)->getTransSides();
	bodyStateSolver->computeTotalTorque({ sides[0].center, sides[2].center, sides[4].center });
	bodyStateSolver->updateState(dt);
}

std::vector<SceneObject::interaction> Box::calculateInteractions(const std::vector<SceneObject*>& sceneObjects) {
	std::vector<interaction> result;
	for (SceneObject* sceneObject : sceneObjects) {
		if (sceneObject != this) {
			Collider* otherCollider = sceneObject->getCollider();
			auto contactData = collider->collidesWith(*otherCollider);

			if (contactData.size() > 0) {
				//rollbackContactWith(sceneObject);
				result.push_back(std::make_tuple(this, sceneObject, contactData));
			}
		}
	}
	return result;
}
