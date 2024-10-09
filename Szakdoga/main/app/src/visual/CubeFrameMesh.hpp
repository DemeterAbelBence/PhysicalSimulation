#pragma once

#include <glm.hpp>
#include <array>

#include "Mesh.hpp"
#include "../util/Camera.hpp"

class CubeFrameMesh : public Mesh {

private:
	float edgeLength;
	std::array<glm::vec3, 8> points;
	std::vector<glm::vec3> vertexData;

private:
	static GpuProgram* staticProgram;

private:
	void calculateBufferData();
	void createPoints();

public:
	CubeFrameMesh(float e);

	void create() override;
	void draw(const Camera& camera) const override;
};