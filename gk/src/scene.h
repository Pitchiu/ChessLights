#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "weather.h"

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

class Scene
{
private:
	Scene();
	~Scene();
	static Scene* mScene;

	Camera camera;
	GLFWwindow* window;
	WeatherConditionsController conditionsController;

	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);

public:
	
	static Scene* getInstance();

	bool firstMouse = true;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	glm::vec3 lightPos = glm::vec3(50.0f, 50.0f, 50.0f);

	float dayNightCycle = 0.0f;

	void run();


	friend void framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height);
	friend void mouseCallbackHandle(GLFWwindow* window, double xposIn, double yposIn);
	friend void scrollCallbackHandle(GLFWwindow* window, double xoffset, double yoffset);
};
