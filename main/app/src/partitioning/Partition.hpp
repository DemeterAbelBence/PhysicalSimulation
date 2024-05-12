#pragma once

#include "Cluster.hpp"
#include "../SceneObject.hpp"

class Partition {

private:
	static const unsigned int level = 6;
	static const unsigned int width_count = 6;
	static const unsigned int height_count = 2;
	static const unsigned int length_count = 6;
	const float size = 90.0f;

	Cluster* clusters[level][level][level];

private:
	inline bool correctIndex(unsigned int index, unsigned int range) { return (index >= 0 && index < range); }

public:
	void addObject(SceneObject* object);
	void initializeCluserNeighbours();
	void initializeClusters();
	void emptyClusters();

	void update();
	void draw(const Camera& camera) const;

	~Partition();
};
