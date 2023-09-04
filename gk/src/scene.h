#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "weather.h"
#include "shader_m.h"
#include "model.h"
#include "object.h"

const glm::vec3 staticCameraPos = glm::vec3(11.0f, 5.0f, 11.0f);
const float staticCameraPitch = -17.0f;
const float staticCameraYaw = -138.0f;

const glm::vec3 trackingCameraPos = glm::vec3(15.0f, 7.0f, 0.0f);
const float trackingCameraBasePitch = -20.0f;
const float trackingCameraBaseYaw = -212.0f;
const float trackingCameraExtraPitch = 5.0f;
const float trackingCameraExtraYaw = 30.0f;

const glm::vec3 POVCameraPos = glm::vec3(0.75f, 6.01f, 7.87f);



class Scene
{
private:
	Scene();
	~Scene();
	static Scene* mScene;

	Camera camera;
	GLFWwindow* window;

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window, ConditionsController& controller);
	void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
	void changeCamera();

	bool wasPressed = false;

	enum CameraMode
	{
		Static,
		POV,
		Tracking,
		Free
	};

	CameraMode cameraMode = Static;
	
public:
	
	static Scene* getInstance();

	bool firstMouse = true;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;

	void run();

	friend void framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height);
	friend void mouseCallbackHandle(GLFWwindow* window, double xposIn, double yposIn);
	friend void scrollCallbackHandle(GLFWwindow* window, double xoffset, double yoffset);
};
