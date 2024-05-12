#include "Scene.hpp"

void Scene::initializeEvents() {
	using namespace KeyboardInput;

	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_DOWN), &Camera::moveUp, -1 ));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_UP), &Camera::moveUp, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_LEFT), &Camera::moveRight, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_RIGHT), &Camera::moveRight, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_Q), &Camera::moveForward, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_W), &Camera::moveForward, 1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_E), &Camera::rotate, -1));
	cameraTransformationEventHandler
		.addEvent(Event<Camera, int>( new glfw::Key(GLFW_KEY_R), &Camera::rotate, 1));

	float speed = 0.08f;
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>( 
			new glfw::Key(GLFW_KEY_DOWN), &SceneObject::translate, glm::vec3(0.0f, speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
			new glfw::Key(GLFW_KEY_UP), &SceneObject::translate, glm::vec3(0.0f, -speed, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
			new glfw::Key(GLFW_KEY_LEFT), &SceneObject::translate, glm::vec3(speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
			new glfw::Key(GLFW_KEY_RIGHT), &SceneObject::translate, glm::vec3(-speed, 0.0f, 0.0f)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
			new glfw::Key(GLFW_KEY_Q), &SceneObject::translate, glm::vec3(0.0f, 0.0f, -speed)));
	objectTransformationEventHandler.addEvent(Event<SceneObject, glm::vec3>(
			new glfw::Key(GLFW_KEY_W), &SceneObject::translate, glm::vec3(0.0f, 0.0f, speed)));
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

	boxTexture = new Texture("boxTexture");
	boxTexture->load("main/app/images/box1.jpg");

	floorTexture = new Texture("floorTexture");
	floorTexture->load("main/app/images/floor.jpg");

	sunTexture = new Texture("sunTexture");
	sunTexture->load("main/app/images/sun.jpg");

	pinkTexture = new Texture("pinkTexture");
	pinkTexture->load("main/app/images/pink.jpg");

	footballTexture = new Texture("footballTexture");
	footballTexture->load("main/app/images/football.jpg");

	basicMaterial = new Material(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.3f, 0.3f, 0.3f),
		glm::vec3(0.2f, 0.2f, 0.2f),
		1.0f
	);
}

void Scene::dropBalls() {
	float radius = 0.5f;
	float scatter = 3.0f;
	unsigned int n = 10;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			Sphere* s = new Sphere(radius, 0.03f);
			glm::vec3 pos = glm::vec3(-25.0, 0.0f, -25.0f) + glm::vec3(i * scatter, 12.0f, j * scatter);
			glm::vec3 vel = glm::vec3(2 * rand() / (float)RAND_MAX, 2 * rand() / (float)RAND_MAX, 2 * rand() / (float)RAND_MAX);

			s->setMeshResources(phongShader, footballTexture, basicMaterial);
			s->translate(pos);
			s->getPhysicsBody()->setVelocity(vel);
			s->create();

			sceneObjects.push_back(s);
			partition->addObject(s);
		}
	}
}

Scene::Scene() {
	object_index = 1;
	move_camera = true;

	camera = new Camera();

	initializeResources();
	initializeEvents();
}

void Scene::getNextObject() {
	object_index = 1;
}

void Scene::setPrimitiveType(unsigned int type) {
	for (SceneObject* g : sceneObjects)
		g->getMesh()->setPrimitive(type);
}

void Scene::create() {
	partition = new Partition();
	partition->initializeClusters();
	partition->initializeCluserNeighbours();

	sun = new Sun(2.0f);
	sun->setMeshResources(textureShader, sunTexture, basicMaterial);
	sun->translate(glm::vec3(-10.0f, 10.0f, 10.0f));
	sun->create();
	sceneObjects.push_back(sun);

	Box* b0 = new Box(70.0f, 5.0f, 70.0f, 0.0f);
	b0->setMeshResources(phongShader, floorTexture, basicMaterial);
	b0->translate(glm::vec3(0.0f, -6.0f, 0.0f));
	b0->create();
	sceneObjects.push_back(b0);
	partition->addObject(b0);

	Box* b1 = new Box(7.0f, 7.0f, 7.0f, 0.1f);
	b1->setMeshResources(phongShader, boxTexture, basicMaterial);
	b1->translate(glm::vec3(10.0f, 6.0f, 10.0f));
	b1->create();
	sceneObjects.push_back(b1);
	partition->addObject(b1);

	Box* b2 = new Box(5.0f, 5.0f, 5.0f, 0.1f);
	b2->setMeshResources(phongShader, boxTexture, basicMaterial);
	b2->translate(glm::vec3(10.0f, 6.0f, -10.0f));
	b2->create();
	sceneObjects.push_back(b2);
	partition->addObject(b2);

	dropBalls();
}

void Scene::draw() const {
	//partition->draw(*camera);
	for (SceneObject* s : sceneObjects)
		s->draw(*camera);
}

void Scene::update(GLFWwindow* window) {
	if (move_camera) {
		if (cameraTransformationEventHandler.handleEvents(window, camera))
			camera->resetView();
	}
	sun->updateInteractions(sceneObjects);

	partition->update();

	/*for (SceneObject* s : sceneObjects)
		s->updateInteractions(sceneObjects);

	for (SceneObject* s : sceneObjects)
		s->updateTransformations();*/
}

Scene::~Scene() {
	//partition->emptyClusters();

	for(SceneObject* s : sceneObjects) {
		s->getMesh()->setProgram(nullptr);
		s->getMesh()->setTexture(nullptr);
		s->getMesh()->setMaterial(nullptr);
	}

	delete camera;
	delete partition;

	delete phongShader;
	delete textureShader;

	delete boxTexture;
	delete floorTexture;
	delete sunTexture;
	delete pinkTexture;
	delete footballTexture;

	delete basicMaterial;
}
