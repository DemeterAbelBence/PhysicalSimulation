#include "CubeFrameMesh.hpp"

void CubeFrameMesh::createPoints() {
	float e = edgeLength / 2.0f;

	//bottom
	points[0] = glm::vec3(-e, -e, e);
	points[1] = glm::vec3(e, -e, e);
	points[2] = glm::vec3(e, -e, -e);
	points[3] = glm::vec3(-e, -e, -e);

	//top
	points[4] = glm::vec3(-e, e, e);
	points[5] = glm::vec3(e, e, e);
	points[6] = glm::vec3(e, e, -e);
	points[7] = glm::vec3(-e, e, -e);
}

void CubeFrameMesh::calculateBufferData() {
	//bottom
	vertexData.push_back(points[0]);
	vertexData.push_back(points[1]);

	vertexData.push_back(points[1]);
	vertexData.push_back(points[2]);

	vertexData.push_back(points[2]);
	vertexData.push_back(points[3]);

	vertexData.push_back(points[3]);
	vertexData.push_back(points[0]);

	//top
	vertexData.push_back(points[4]);
	vertexData.push_back(points[5]);

	vertexData.push_back(points[5]);
	vertexData.push_back(points[6]);

	vertexData.push_back(points[6]);
	vertexData.push_back(points[7]);

	vertexData.push_back(points[7]);
	vertexData.push_back(points[4]);

	//side
	vertexData.push_back(points[0]);
	vertexData.push_back(points[4]);

	vertexData.push_back(points[1]);
	vertexData.push_back(points[5]);

	vertexData.push_back(points[2]);
	vertexData.push_back(points[6]);

	vertexData.push_back(points[3]);
	vertexData.push_back(points[7]);

	const void* bufferData = (const void*)&vertexData[0];
	unsigned int bufferSize = vertexData.size() * sizeof(glm::vec3);

	vertexBuffer.setData(bufferData, bufferSize);
	vertexBufferLayout.push(GL_FLOAT, 3);
	vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);
}

CubeFrameMesh::CubeFrameMesh(float e) : Mesh() {
	edgeLength = e;
}

void CubeFrameMesh::create() {
	if (!staticProgram) {
		staticProgram = new GpuProgram("cube_frame_program");
		staticProgram->readVertexSource("main/app/shaders/cubeFrameVertex.shader");
		staticProgram->readFragmentSource("main/app/shaders/cubeFrameFragment.shader");
		staticProgram->createProgram();
	}

	createPoints();
	calculateBufferData();
}

void CubeFrameMesh::draw(const Camera& camera) const {
	vertexArray.bind();

	staticProgram->bind();
	staticProgram->setMat4("MI", transformation->makeModelInverseMatrix());
	staticProgram->setMat4("M", transformation->makeModelMatrix());
	staticProgram->setMat4("V", camera.getView());
	staticProgram->setMat4("P", camera.getProjection());
	setAllUniforms();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
	glDrawArrays(GL_LINES, 0, vertexData.size());
}

GpuProgram* CubeFrameMesh::staticProgram = nullptr;