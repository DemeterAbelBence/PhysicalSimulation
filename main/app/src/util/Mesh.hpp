#pragma once

#include "gl/VertexArray.hpp"
#include "gl/GpuProgram.hpp"
#include "tex/Texture.hpp"
#include "datatypes/Material.hpp"
#include "Transformation.hpp"
#include "Camera.hpp"

#include <glm.hpp>

class Mesh {

protected:
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	VertexBufferLayout vertexBufferLayout;

	Transformation transformation;

	GpuProgram* program;
	Material* material;
	Texture* texture;

	unsigned int primitiveType;
	unsigned int meshFrame;

public:
	Mesh();

	void setNewProgram(const GpuProgram& _program);
	void setNewMaterial(const Material& _material);
	void setNewTexture(const Texture& _texture);

	inline void setProgram(GpuProgram* _program) { program = _program; }
	inline void setMaterial(Material* _material) { material = _material; }
	inline void setTexture(Texture* _texture) { texture = _texture; }

	inline GpuProgram* getProgram() const { return program; }
	inline Material* getMaterial() const { return material; }
	inline Texture* getTexture() const { return texture; }

	inline Transformation& getTransformation() { return transformation; }

	inline void setPrimitive(unsigned int type) { primitiveType = type; }
	inline unsigned int getPrimitiveType() const { return primitiveType; }

	void setAllUniforms() const;
	void drawMeshFrame(const Camera& camera);
	
	virtual void create() = 0;
	virtual void draw(const Camera& camera) const = 0;

	virtual ~Mesh();
};