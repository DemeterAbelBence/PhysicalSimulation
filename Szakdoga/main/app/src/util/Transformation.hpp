#pragma once

#include <glm.hpp>

#include "gl/GpuProgram.hpp"

class Transformation {

private:
	glm::mat4 T;
	glm::mat4 R;
	glm::mat4 S;

public:
	Transformation();

	inline void setTranslationMarix(const glm::mat4& value) { T = value; }
	inline void setRotationMarix(const glm::mat4& value) { R = value; }
	inline void setScaleMatrix(const glm::mat4& value) { S = value; }

	inline glm::mat4 getTranslationMatrix() const { return T; }
	inline glm::mat4 getRotationMatrix() const { return R; }

	static glm::mat4 makeTranslationMatrix(glm::vec3 translation);
	static glm::mat4 makeScaleMatrix(glm::vec3 scaling);
	static glm::mat4 makeRotationMatrix(glm::vec3 rotationAxis, float rotationAngle);

	glm::mat4 makeModelMatrix() const;
	glm::mat4 makeModelInverseMatrix() const;
};