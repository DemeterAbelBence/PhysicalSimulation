#include "Terrain.hpp"

Terrain::Terrain(float size, unsigned int rezolution) {
	mesh = new PatchSurface(size, rezolution);
	collider = new NullCollider();

	auto bodyData = RigidBody::BodyData{
		0.0f,					// InvMass
		glm::mat3(0.0f),        // Ibody
		glm::mat3(0.0f),        // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f, 0.0f, 0.0f),        // force
		glm::vec3(0.0f)                      // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(bodyData);

	transformation = new Transformation();
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void Terrain::setTesselationParameters(unsigned int min_t, unsigned int max_t, float min_d, float max_d) {
	minimal_tesselation = min_t;
	maximal_tesselation = max_t;
	minimal_distance = min_d;
	maximal_distance = max_d;
}

void Terrain::resetBodyState() { }

void Terrain::updateBodyState(float dt) { }

std::vector<SceneObject::interaction> Terrain::update(const std::vector<SceneObject*>& gameObjects) {
	GpuProgram* program = mesh->getProgram();

	program->bind();
	program->setFloat("amplitude_factor", amplitude);
	program->setFloat("phase_factor", phase);
	program->setInt("iterations", iterations);

	program->setFloat("MIN_DIST", minimal_distance);
	program->setFloat("MAX_DIST", maximal_distance);
	program->setInt("MIN_TESS", minimal_tesselation);
	program->setInt("MAX_TESS", maximal_tesselation);

	return std::vector<interaction>();
}


