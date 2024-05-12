#pragma once 

#include "SceneObject.hpp"
#include "visual/SphereMesh.hpp"
#include "collision/SphereCollider.hpp"

class Sphere : public SceneObject {
	
public:
	Sphere(float radius, float inverseMass);
	void updateTransformations() override;
	void updateInteractions(const std::vector<SceneObject*>& sceneObjects) override;
};