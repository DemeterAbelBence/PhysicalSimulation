#pragma once

#include "src/util/gl/GpuProgram.hpp"
#include "src/util/gl/TesselationProgram.hpp"
#include "src/util/gl/VertexArray.hpp"
#include "src/util/glfw/KeyboardInput.hpp"
#include "src/util/glfw/Time.hpp"
#include "src/util/datatypes/Material.hpp"
#include "src/util/Mesh.hpp"
#include "src/util/Camera.hpp"

#include "src/partitioning/Partition.hpp"

#include "src/Box.hpp"
#include "src/Sun.hpp"
#include "src/Sphere.hpp"

class Scene {

private:
	Camera* camera;
	Partition* partition;
	std::vector<SceneObject*> sceneObjects;
	Sun* sun;

	GpuProgram* phongShader;
	GpuProgram* textureShader;

	Texture* boxTexture;
	Texture* floorTexture;
	Texture* sunTexture;
	Texture* pinkTexture;
	Texture* footballTexture;

	Material* basicMaterial;

	KeyboardInput::EventHandler<Camera, int> cameraTransformationEventHandler;
	KeyboardInput::EventHandler<SceneObject, glm::vec3> objectTransformationEventHandler;

private:
	unsigned int object_index;
	bool move_camera;

private:
	void initializeEvents();
	void initializeResources();
	void dropBalls();

public:
	Scene();

	inline Camera* getCamera() const { return camera; }
	inline void setMoveCamera(bool value) { move_camera = value; }
	inline std::vector<SceneObject*> getGameObjects() const { return sceneObjects; }
	inline SceneObject* getCurrentGameObject() { return sceneObjects[object_index]; }

	void getNextObject();
	void setPrimitiveType(unsigned int type);

	void create();
	void draw() const;
	void update(GLFWwindow* window);

	~Scene();	
};