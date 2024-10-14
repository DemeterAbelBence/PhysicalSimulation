#pragma once

#include "SceneObject.hpp"
#include "visual/PatchSurface.hpp"
#include "collision/NullCollider.hpp"

class Terrain : public SceneObject {
private:
	float amplitude;
	float phase;
	unsigned int iterations;

	unsigned int minimal_tesselation = 0;
	unsigned int maximal_tesselation = 0;

	float minimal_distance = 0.0f;
	float maximal_distance = 0.0f;

public:
	Terrain(float size, unsigned int rezolution);

	inline void setAmplitude(float value) { amplitude = value; }
	inline void setPhase(float value) { phase = value; }
	inline void setIterations(unsigned int value) { iterations = value; }

	void setTesselationParameters(unsigned int min_t, unsigned int max_t, float min_d, float max_d);

	void resetBodyState() override;
	void updateBodyState(float dt) override;
	std::vector<interaction> update(const std::vector<SceneObject*>& gameObjects) override;
};