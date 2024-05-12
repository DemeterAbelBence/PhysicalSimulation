#pragma once

#include <glm.hpp>

#include "gl/GpuProgram.hpp"

class Transformation {

private:
	glm::vec3 translation;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	float rotationAngle = 0.0f;

public:
	Transformation();
	Transformation(const Transformation& t);

	inline glm::vec3 getTranslation() const { return translation; }
	inline glm::vec3 getScaling() const { return scaling; }
	inline glm::vec3 getRotationAxis() const { return rotationAxis; }
	inline float getRotationAngle() const { return rotationAngle; }

	inline void setRotationAngle(float value) { rotationAngle = value; }
	inline void setRotationAxis(glm::vec3 value) { rotationAxis = value; }

	void translate(glm::vec3 t);
	void scale(float s);
	void rotate(float angle);
	void resetTransformations();

	glm::mat4 makeModelMatrix() const;
	glm::mat4 makeModelInverseMatrix() const;

	glm::mat4 makeTranslationMatrix() const;
	glm::mat4 makeScaleMatrix() const;
	glm::mat4 makeRotationMatrix() const;

	glm::mat4 makeTranslationInverseMatrix() const;
	glm::mat4 makeScaleInverseMatrix() const;
	glm::mat4 makeRotationInverseMatrix() const;
};