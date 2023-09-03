#include "scene.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Scene* Scene::mScene = nullptr;
std::vector<std::string> Shader::commonCode = std::vector<std::string>();



void framebufferSizeCallbackHandle(GLFWwindow* window, int width, int height)
{
    Scene* scene = Scene::getInstance();
    scene->framebufferSizeCallback(window, width, height);
}

void mouseCallbackHandle(GLFWwindow* window, double xposIn, double yposIn)
{
    Scene* scene = Scene::getInstance();
    scene->mouseCallback(window, xposIn, yposIn);
}

void scrollCallbackHandle(GLFWwindow* window, double xoffset, double yoffset)
{
    Scene* scene = Scene::getInstance();
    scene->scrollCallback(window, xoffset, yoffset);
}


Scene::Scene() : camera(glm::vec3(0.0f, 0.0f, 0.0f))
{
    // Scene setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ChessLights", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallbackHandle);
    glfwSetCursorPosCallback(window, mouseCallbackHandle);
    glfwSetScrollCallback(window, scrollCallbackHandle);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glewInit();

    //stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
}

Scene* Scene::getInstance()
{
    if (mScene == nullptr)
        mScene = new Scene();
    return mScene;
}

void Scene::run()
{
    Shader::addCommonFile("res\\shaders\\light.glsl");
    // build and compile shaders
    Shader boardShader("res\\shaders\\board.vs", "res\\shaders\\board.fs");
    Shader whiteKingShader("res\\shaders\\king.vs", "res\\shaders\\king.fs");

    // load models
    Model boardModel("res/board/board.obj");
    Model whiteKingModel("res/king/king.obj");

    //std::vector<IluminatedObject> objects;
    Board board(boardShader, boardModel);
    WhiteKing whiteKing(whiteKingShader, whiteKingModel);

    LightProperty lightProperty;
    ConditionsController conditionsController;


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, conditionsController);

        conditionsController.updateTime();
        auto background = conditionsController.getBackgroundColor();
        glClearColor(background.r, background.g, background.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        board.draw(lightProperty, camera, conditionsController);
        whiteKing.draw(lightProperty, camera, conditionsController);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

Scene::~Scene()
{
    glfwTerminate();
}


void Scene::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Scene::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Scene::mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void Scene::processInput(GLFWwindow* window, ConditionsController &controller)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if(!wasPressed)
            controller.changeFog();
        wasPressed = true;

    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
        wasPressed = false;
    //if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    //    x -= 0.01f;
    //if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    //    y += 0.01f;
    //if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    //    y -= 0.01f;
    //if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    //    z += 0.01f;
    //if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    //    z -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}
