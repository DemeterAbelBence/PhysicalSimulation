#include "Mesh.hpp"

void Mesh::drawMeshFrame(const Camera& camera) {
	transformation.scale(0.2f);
	meshFrame = 1;
	draw(camera);
	transformation.scale(-0.2f);
	meshFrame = 0;
}

Mesh::Mesh() {
	program = nullptr; 
	texture = nullptr;
	material = nullptr;

	meshFrame = 0;
	primitiveType = GL_TRIANGLES;
}

void Mesh::setNewProgram(const GpuProgram& _program) {
	if (program)
		delete program;
	program = new GpuProgram(_program);
}

void Mesh::setNewMaterial(const Material& _material) {
	if (material)
		delete material;
	material = new Material(_material);
}

void Mesh::setNewTexture(const Texture& _texture) {
	if (texture)
		delete texture;
	texture = new Texture(_texture);
}

void Mesh::setAllUniforms() const {
	if (program) {
		program->bind();
		if (material) {
			program->setVec3("material.ambient", material->ambient);
			program->setVec3("material.diffuse", material->diffuse);
			program->setVec3("material.specular", material->specular);
			program->setFloat("material.shininess", material->shininess);
		}
		if(texture) {
			glActiveTexture(GL_TEXTURE0);
			texture->bind();
			glUniform1i(glGetUniformLocation(program->getProgramId(),
						texture->getName().c_str()), 0);
		}
		program->setInt("meshFrame", meshFrame);
	}
}

Mesh::~Mesh() {
	if(program)
		delete program;
	if(material)
		delete material;
	if(texture)
		delete texture;
}
