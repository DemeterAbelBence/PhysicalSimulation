#pragma once

#include "../SceneObject.hpp"
#include "../visual/CubeFrameMesh.hpp"
#include "../collision/CuboidCollider.hpp"
#include "../collision/SphereCollider.hpp"
#include "../collision/NullCollider.hpp"

class Cluster {

private:
	float edge_length;

	std::vector<SceneObject*> objects;
	std::vector<Cluster*> neighbours;
	
	CubeFrameMesh* mesh;
	CuboidCollider* collider;

public:
	Cluster(float e);

	inline void addObject(SceneObject* object) { objects.push_back(object); }
	inline void addNeighbour(Cluster* cluster) { neighbours.push_back(cluster); }
	inline const std::vector<SceneObject*>& getObjects() const { return objects; }

	void translate(glm::vec3 t);
	bool containsObject(SceneObject* object);
	void updateClusterContent();
	void emptyCluster();
	void setObjectsUpdatedValue(bool value);

	void updateObjectInteractions() const;
	void updateObjectTransformations() const;
	void drawFrame(const Camera& camera) const;

	~Cluster();
};