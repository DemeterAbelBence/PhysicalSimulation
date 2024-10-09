#include "Scene.hpp"

void Scene::initializeEvents() {
	using namespace KeyboardInput;

	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_S), &Camera::moveUp, -1 ));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_W), &Camera::moveUp, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_A), &Camera::moveRight, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_D), &Camera::moveRight, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_LEFT_SHIFT), &Camera::moveForward, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_LEFT_CONTROL), &Camera::moveForward, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_E), &Camera::rotate, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_Q), &Camera::rotate, 1));

	resetObjectEvents(0.05f);
	resetSimulationEvents(0.01f);
}

void Scene::initializeResources() {
	phongShader = new GpuProgram("phong_shader");
	phongShader->readVertexSource("main/app/shaders/phongVertex.shader");
	phongShader->readFragmentSource("main/app/shaders/phongFragment.shader");
	phongShader->createProgram();

	textureShader = new GpuProgram("texture_shader");
	textureShader->readFragmentSource("main/app/shaders/textureFragment.shader");
	textureShader->readVertexSource("main/app/shaders/textureVertex.shader");
	textureShader->createProgram();

	Texture* texture;

	texture = new Texture("boxTexture");
	texture->load("main/app/images/box1.jpg");
	textures.push_back(texture);

	texture = new Texture("floorTexture");
	texture->load("main/app/images/floor.jpg");
	textures.push_back(texture);

	texture = new Texture("sunTexture");
	texture->load("main/app/images/sun.jpg");
	textures.push_back(texture);

	texture = new Texture("pinkTexture");
	texture->load("main/app/images/pink.jpg");
	textures.push_back(texture);

	materials.push_back(new Material(
		glm::vec3(0.3f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.0f, 0.3f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.0f, 0.0f, 0.3f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));

	materials.push_back(new Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		10.0f
	));
}

Scene::Scene() {
	object_index = 0;
	move_camera = true;

	camera = new Camera();

	initializeResources();
	initializeEvents();
}

void Scene::getNextObject() {
	if (++object_index == sceneObjects.size()) {
		object_index = 0;
	}
}

void Scene::setPrimitiveType(unsigned int type) {
	for (SceneObject* g : sceneObjects)
		g->getMesh()->setPrimitive(type);
}

void Scene::setObjectRotation(glm::vec3 axis, float angle) {
	glm::mat4 R = Transformation::makeRotationMatrix(axis, angle);
	sceneObjects[object_index]->setRotation(R);
}

void Scene::resetObjectEvents(float speed) {
	using namespace KeyboardInput;

	objectTransformationEventHandler.removeAllEvents();

	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_W), &SceneObject::translate, glm::vec3(0.0f, speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_S), &SceneObject::translate, glm::vec3(0.0f, -speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_D), &SceneObject::translate, glm::vec3(speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_A), &SceneObject::translate, glm::vec3(-speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_LEFT_SHIFT), &SceneObject::translate, glm::vec3(0.0f, 0.0f, -speed)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
		new glfw::Key(GLFW_KEY_LEFT_CONTROL), &SceneObject::translate, glm::vec3(0.0f, 0.0f, speed)));
}

void Scene::resetSimulationEvents(float delta_time) {
	using namespace KeyboardInput;

	simulationEventHandler.removeAllEvents();

	simulationEventHandler.addEvent(Event<Scene, float>(
		new glfw::Key(GLFW_KEY_LEFT), &Scene::updateSimulation, -delta_time));

	simulationEventHandler.addEvent(Event<Scene, float>(
		new glfw::Key(GLFW_KEY_RIGHT), &Scene::updateSimulation, delta_time));
}

void Scene::rollbackSimulation() {
	for (SceneObject* s : sceneObjects) {
		s->resetBodyState();
	}
}

void Scene::create() {
	sun = new Sun(2.0f);
	sun->setMeshResources(textureShader, textures[SUN], materials[BASIC]);
	sun->translate(glm::vec3(20.0f, 25.0f, 20.0f));
	sun->create();
	sceneObjects.push_back(sun);

	Box* b0 = new Box(70.0f, 3.0f, 70.0f, true);
	b0->setMeshResources(phongShader, textures[FLOOR], materials[BASIC]);
	b0->translate(glm::vec3(0.0f, -5.0f, 0.0f));
	b0->getBodyStateSolver()->makeStateInitial();
	b0->create();
	sceneObjects.push_back(b0);

	Box* b1 = new Box(7.0f, 7.0f, 7.0f, false);
	b1->setMeshResources(phongShader, textures[BOX], materials[BASIC]);
	b1->translate(glm::vec3(10.0f, 10.0f, 10.0f));
	b1->setRotation(Transformation::makeRotationMatrix(glm::normalize(glm::vec3(0.0f, 0.7f, 0.4f)), -0.7f * 3.14159f));
	b1->getBodyStateSolver()->Body.P = glm::vec3(-5.0f, 0.0f, 0.0f);
	b1->getBodyStateSolver()->makeStateInitial();
	b1->create();
	sceneObjects.push_back(b1);

	/*Box* b1 = new Box(7.0f, 7.0f, 7.0f, false);
	b1->setMeshResources(phongShader, textures[BOX], materials[RED]);
	b1->translate(glm::vec3(10.0f, 25.0f, 10.0f));
	b1->setRotation(Transformation::makeRotationMatrix(glm::normalize(glm::vec3(0.0f, 0.7f, 0.4f)), -0.7f * 3.14159f));
	b1->getBodyStateSolver()->Body.P = glm::vec3(-25.0f, 0.0f, 0.0f);
	b1->getBodyStateSolver()->makeStateInitial();
	b1->create();
	sceneObjects.push_back(b1);

	Box* b2 = new Box(7.0f, 7.0f, 7.0f, false);
	b2->setMeshResources(phongShader, textures[BOX], materials[BLUE]);
	b2->translate(glm::vec3(-10.0f, 25.0f, 10.0f));
	b2->setRotation(Transformation::makeRotationMatrix(glm::normalize(glm::vec3(4.0f, 0.7f, 0.0f)), 0.4f * 3.14159f));
	b2->getBodyStateSolver()->Body.P = glm::vec3(25.0f, 0.0f, 0.0f);
	b2->getBodyStateSolver()->makeStateInitial();
	b2->create();
	sceneObjects.push_back(b2);*/
}

void Scene::draw() const {
	for (SceneObject* s : sceneObjects)
		s->draw(*camera);

	sceneObjects[object_index]->getMesh()->drawMeshFrame(*camera);
}

void Scene::updateSimulation(float delta_time) {
	SceneObject::setDeltaTime(delta_time);
	for(SceneObject* sceneObject : sceneObjects) {
		sceneObject->updateBodyState(delta_time);
	}
}

void Scene::update(GLFWwindow* window) {
	if (move_camera) {
		if (cameraTransformationEventHandler.handleEvents(window, camera)) {
			camera->resetView();
		}
	}
	else {
		objectTransformationEventHandler.handleEvents(window, sceneObjects[object_index]);
	}

	std::vector<SceneObject::interaction> interactions;
	for (SceneObject* s : sceneObjects) {
		auto i = s->calculateInteractions(sceneObjects);
		interactions.insert(interactions.end(), i.begin(), i.end());
	}
	CollisionHandler::HandleCollisions(interactions, *camera);

	simulationEventHandler.handleEvents(window, this);

	for (SceneObject* s : sceneObjects) {
		s->updateTransformations();
	}
}

Scene::~Scene() {
	for(SceneObject* s : sceneObjects) {
		s->getMesh()->setProgram(nullptr);
		s->getMesh()->setTexture(nullptr);
		s->getMesh()->setMaterial(nullptr);
	}

	delete phongShader;
	delete textureShader;

	delete camera;
}
