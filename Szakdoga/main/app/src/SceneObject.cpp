#include "SceneObject.hpp"

float SceneObject::delta_time = 0.01f;

bool SceneObject::enable_debug = true;

SceneObject::SceneObject() {
	collider = nullptr;
	mesh = nullptr;
	bodyStateSolver = nullptr;
	transformation = nullptr;
}

void SceneObject::translate(glm::vec3 translation) {
	bodyStateSolver->Body.X += translation;
	updateTransformations();
}

void SceneObject::setRotation(glm::mat4 R) {
	bodyStateSolver->Body.R = glm::mat3(R);
	updateTransformations();
}

void SceneObject::setMeshResources(GpuProgram* program, Texture* texture, Material* material) {
	mesh->setProgram(program);
	mesh->setMaterial(material);
	mesh->setTexture(texture);
}

void SceneObject::create() {
	mesh->create();
}

void SceneObject::draw(const Camera& camera) const {
	mesh->draw(camera);

	if (enable_debug) {
		if (collider->type == Collider::C_CUBOID)
		{
			CuboidCollider* c = dynamic_cast<CuboidCollider*>(collider);

			auto p = c->getTransPoints();
			auto points = std::vector<glm::vec3>(p.begin(), p.end());
			DebugDrawer::setMode(GL_POINTS);
			DebugDrawer::setVertexData(points);
			DebugDrawer::setOverrideZ(0);
			DebugDrawer::draw(camera, glm::vec3(1.0f, 0.0f, 0.0f));

			std::vector<std::vector<glm::vec3>> sideData;
			sideData.push_back(c->getSideDrawData(0));
			sideData.push_back(c->getSideDrawData(1));
			sideData.push_back(c->getSideDrawData(2));
			sideData.push_back(c->getSideDrawData(3));
			sideData.push_back(c->getSideDrawData(4));
			sideData.push_back(c->getSideDrawData(5));

			for (const auto& data : sideData) {
				DebugDrawer::setMode(GL_LINES);
				DebugDrawer::setOverrideZ(0);
				DebugDrawer::setVertexData(data);
				DebugDrawer::draw(camera, glm::vec3(0.0f, 0.4f, 0.4f));
			}
		}
	}
}

void SceneObject::updateTransformations() {
	transformation->setTranslationMarix(bodyStateSolver->Body.X);

	transformation->setR(glm::mat4(
		glm::vec4(bodyStateSolver->Body.R[0], 0.0f),
		glm::vec4(bodyStateSolver->Body.R[1], 0.0f),
		glm::vec4(bodyStateSolver->Body.R[2], 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	));

	collider->updateTransformations();
}

SceneObject::~SceneObject() {	
	delete mesh;
	delete collider;
	delete bodyStateSolver;
	delete transformation;
}
