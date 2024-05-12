#include "SceneObject.hpp"

SceneObject::SceneObject() {
	collider = nullptr;
	mesh = nullptr;
	physicsBody = nullptr;

	dragged = false;
	updated = false;
}

void SceneObject::setMeshResources(GpuProgram* program, Texture* texture, Material* material) { 
	mesh->setProgram(program);
	mesh->setMaterial(material);
	mesh->setTexture(texture);
}

void SceneObject::translate(glm::vec3 t) {
	mesh->getTransformation().translate(t);
	physicsBody->translate(t);
	collider->translate(t);
}

void SceneObject::scale(float s) {
	mesh->getTransformation().scale(s);
	collider->scale(s);
}

void SceneObject::rotate(glm::vec3 axis, float angle) {}

void SceneObject::create() {
	mesh->create();
}

void SceneObject::draw(const Camera& camera) const {
	mesh->draw(camera);
}

SceneObject::~SceneObject() {	
	delete mesh;
	delete collider;
	delete physicsBody;
}
