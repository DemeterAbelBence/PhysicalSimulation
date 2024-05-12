#include "Cluster.hpp"

Cluster::Cluster(float e) {
	edge_length = e;

	mesh = new CubeFrameMesh(e);
	mesh->create();

	collider = new CuboidCollider(e, e, e);
}

void Cluster::updateObjectInteractions() const {
	for (SceneObject* s : objects) {
		if (!s->getUpdated()) {
			s->updateInteractions(objects);
			s->setUpdated(true);
		}
	}
}

void Cluster::updateObjectTransformations() const {
	for (SceneObject* s : objects)
		s->updateTransformations();
}

void Cluster::drawFrame(const Camera& camera) const {
	mesh->draw(camera);
}

void Cluster::translate(glm::vec3 t) {
	collider->translate(t);
	mesh->getTransformation().translate(t);
}

bool Cluster::containsObject(SceneObject* object) {
	Collider* objectCollider = object->getCollider();
	if (static_cast<Collider*>(collider)->collidesWith(*objectCollider) != nullptr)
		return true;
	else
		return false;
}

void Cluster::updateClusterContent() {
	if (objects.empty())
		return;

	std::vector<SceneObject*> notInCluster;
	for (SceneObject* s : objects) {
		if (!containsObject(s))
			notInCluster.push_back(s);
	}

	for (SceneObject* s : notInCluster) {
		auto iterator = std::find(objects.begin(), objects.end(), s);
		if (iterator != objects.end()) {
			objects.erase(iterator);
			for (Cluster* c : neighbours) {
				if (c->containsObject(s))
					c->addObject(s);
			}
		}
	}
}

void Cluster::emptyCluster() {
	for (SceneObject* s : objects)
		s = nullptr;
}

void Cluster::setObjectsUpdatedValue(bool value) {
	for (SceneObject* s : objects)
		s->setUpdated(value);
}

Cluster::~Cluster() {
	delete mesh;
	delete collider;
}

