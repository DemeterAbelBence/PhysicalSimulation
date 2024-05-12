#pragma once 

#include "SceneObject.hpp"
#include "visual/SphereMesh.hpp"
#include "collision/NullCollider.hpp"

class Sun : public SceneObject {
private:
	void setUniformLight(SceneObject* gameObject) const;

public:
	Sun(float radius);
	void updateTransformations() override;
	void updateInteractions(const std::vector<SceneObject*>& sceneObjects) override;
};

