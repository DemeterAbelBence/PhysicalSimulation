#include "Transformation.hpp"

Transformation::Transformation() {
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationAngle = 0.0f;
	rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
}

Transformation::Transformation(const Transformation& t) {
	translation = t.translation;
	scaling = t.scaling;
	rotationAxis = t.rotationAxis;
	rotationAngle = t.rotationAngle;
}

void Transformation::translate(glm::vec3 t) {
	translation += t;
}

void Transformation::scale(float s) {
	scaling += s;
}

void Transformation::rotate(float angle) {
	rotationAngle += angle;
}

glm::mat4 Transformation::makeModelMatrix() const {
	glm::mat4 t = makeTranslationMatrix();
	glm::mat4 s = makeScaleMatrix();
	glm::mat4 r = makeRotationMatrix();

	return s * r * t;
}

glm::mat4 Transformation::makeModelInverseMatrix() const {
	glm::mat4 t = makeTranslationInverseMatrix();
	glm::mat4 s = makeScaleInverseMatrix();
	glm::mat4 r = makeRotationInverseMatrix();

	return t * r * s;
}

void Transformation::resetTransformations() {
	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scaling = glm::vec3(1.0f, 1.0f, 1.0f);
	rotationAngle = 0.0f;
}

glm::mat4 Transformation::makeTranslationMatrix() const {
	glm::mat4 t;
	t[0] = glm::vec4(1.0f, 0.0f, 0.0f, translation.x);
	t[1] = glm::vec4(0.0f, 1.0f, 0.0f, translation.y);
	t[2] = glm::vec4(0.0f, 0.0f, 1.0f, translation.z);
	t[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return t;
}

glm::mat4 Transformation::makeScaleMatrix() const {
	glm::mat4 s;
	s[0] = glm::vec4(scaling.x, 0.0f, 0.0f, 0.0f);
	s[1] = glm::vec4(0.0f, scaling.y, 0.0f, 0.0f);
	s[2] = glm::vec4(0.0f, 0.0f, scaling.z, 0.0f);
	s[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return s;
}

glm::mat4 Transformation::makeRotationMatrix() const {
	glm::mat4 r(1.0f);

	if (rotationAxis == glm::vec3(0.0f))
		return r;

	float a = rotationAngle * 3.14159f / 180.0f;
	glm::vec3 k = rotationAxis;

	float cosA = cos(a);
	float sinA = sin(a);

	r[0][0] = cosA + k.x * k.x * (1.0f - cosA);
	r[0][1] = k.x * k.y * (1.0f - cosA) - k.z * sinA;
	r[0][2] = k.x * k.z * (1.0f - cosA) + k.y * sinA;
	r[0][3] = 0.0f;

	r[1][0] = k.y * k.x * (1.0f - cosA) + k.z * sinA;
	r[1][1] = cosA + k.y * k.y * (1.0f - cosA);
	r[1][2] = k.y * k.z * (1.0f - cosA) - k.x * sinA;
	r[1][3] = 0.0f;

	r[2][0] = k.z * k.x * (1.0f - cosA) - k.y * sinA;
	r[2][1] = k.z * k.y * (1.0f - cosA) + k.x * sinA;
	r[2][2] = cosA + k.z * k.z * (1.0f - cosA);
	r[2][3] = 0.0f;

	r[3][0] = 0.0f;
	r[3][1] = 0.0f;
	r[3][2] = 0.0f;
	r[3][3] = 1.0f;

	return r;
}

glm::mat4 Transformation::makeTranslationInverseMatrix() const {
	glm::mat4 t;
	t[0] = glm::vec4(1.0f, 0.0f, 0.0f, -translation.x);
	t[1] = glm::vec4(0.0f, 1.0f, 0.0f, -translation.y);
	t[2] = glm::vec4(0.0f, 0.0f, 1.0f, -translation.z);
	t[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return t;
}

glm::mat4 Transformation::makeScaleInverseMatrix() const {
	glm::mat4 s;
	s[0] = glm::vec4(1.0f/ scaling.x, 0.0f, 0.0f, 0.0f);
	s[1] = glm::vec4(0.0f, 1.0f / scaling.y, 0.0f, 0.0f);
	s[2] = glm::vec4(0.0f, 0.0f, 1.0f / scaling.z, 0.0f);
	s[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return s;
}

glm::mat4 Transformation::makeRotationInverseMatrix() const {
	glm::mat4 r(1.0f);

	if (rotationAxis == glm::vec3(0.0f))
		return r;

	float a = -rotationAngle * 3.14159f / 180.0f;
	glm::vec3 k = rotationAxis;

	float cosA = cos(a);
	float sinA = sin(a);

	r[0][0] = cosA + k.x * k.x * (1.0f - cosA);
	r[0][1] = k.x * k.y * (1.0f - cosA) - k.z * sinA;
	r[0][2] = k.x * k.z * (1.0f - cosA) + k.y * sinA;
	r[0][3] = 0.0f;

	r[1][0] = k.y * k.x * (1.0f - cosA) + k.z * sinA;
	r[1][1] = cosA + k.y * k.y * (1.0f - cosA);
	r[1][2] = k.y * k.z * (1.0f - cosA) - k.x * sinA;
	r[1][3] = 0.0f;

	r[2][0] = k.z * k.x * (1.0f - cosA) - k.y * sinA;
	r[2][1] = k.z * k.y * (1.0f - cosA) + k.x * sinA;
	r[2][2] = cosA + k.z * k.z * (1.0f - cosA);
	r[2][3] = 0.0f;

	r[3][0] = 0.0f;
	r[3][1] = 0.0f;
	r[3][2] = 0.0f;
	r[3][3] = 1.0f;

	return r;
}
