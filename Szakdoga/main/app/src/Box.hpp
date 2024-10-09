#pragma once

#include <tuple>

#include "SceneObject.hpp"
#include "visual/CuboidMesh.hpp"
#include "collision/CuboidCollider.hpp"

class Box : public SceneObject {
private:
	void rollbackContactWith(SceneObject* other);

public:
	Box(float width, float height, float length, bool stationary);
	void resetBodyState() override;
	void updateBodyState(float dt) override;

	std::vector<interaction> calculateInteractions(const std::vector<SceneObject*>& gameObjects) override;
};