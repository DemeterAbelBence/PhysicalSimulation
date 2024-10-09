#pragma once 

#include "SceneObject.hpp"
#include "visual/SphereMesh.hpp"
#include "collision/NullCollider.hpp"

class Sun : public SceneObject {
private:
	void setUniformLight(SceneObject* gameObject) const;

public:
	Sun(float radius);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	std::vector<interaction> calculateInteractions(const std::vector<SceneObject*>& sceneObjects) override;
};

