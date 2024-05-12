#pragma once

#include "collision/Collider.hpp"
#include "physics/PhysicsBody.hpp"
#include "util/Mesh.hpp"
#include "util/Camera.hpp"

class SceneObject {
protected:
	Mesh* mesh;
	Collider* collider;
	PhysicsBody* physicsBody;	

protected:
	bool dragged;
	bool updated;

public:
	SceneObject();

	inline Mesh* getMesh() const { return mesh; }
	inline Collider* getCollider() const { return collider; }
	inline PhysicsBody* getPhysicsBody() const { return physicsBody; }

	inline void setDragged(bool value) { dragged = value; }
	inline bool getDragged() { return dragged; }
	inline void setUpdated(bool value) { updated = value; }
	inline bool getUpdated() { return updated; }

	void translate(glm::vec3 t);
	void scale(float s);
	void rotate(glm::vec3 axis, float angle);

	void setMeshResources(GpuProgram* program, Texture* texture, Material* material);
	void create();
	void draw(const Camera& camera) const;

	virtual void updateTransformations() = 0;
	virtual void updateInteractions(const std::vector<SceneObject*>& sceneObjects) = 0;

	~SceneObject();
};