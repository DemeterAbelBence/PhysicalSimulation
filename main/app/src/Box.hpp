#pragma once

#include "SceneObject.hpp"
#include "visual/CuboidMesh.hpp"
#include "collision/CuboidCollider.hpp"

class Box : public SceneObject {
public:
	Box(float width, float height, float length, float inverseMass);
	void updateTransformations() override;
	void updateInteractions(const std::vector<SceneObject*>& gameObjects) override;
};