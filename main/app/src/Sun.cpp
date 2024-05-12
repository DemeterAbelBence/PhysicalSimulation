#include "Sun.hpp"

void Sun::setUniformLight(SceneObject* sceneObject) const {
	GpuProgram* program = sceneObject->getMesh()->getProgram();
	program->bind();
	program->setVec3(std::string("light"), physicsBody->getPosition());
}

Sun::Sun(float radius) : SceneObject() {
	mesh = new SphereMesh(30, 30, radius);
	physicsBody = new PhysicsBody(0.0f, 0.0f);
	collider = new NullCollider();
}

void Sun::updateTransformations() {}

void Sun::updateInteractions(const std::vector<SceneObject*>& sceneObjects) {
	for (SceneObject* s : sceneObjects)
		setUniformLight(s);
}
