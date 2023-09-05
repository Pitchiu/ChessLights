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


Scene::Scene()
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

void calculatePitchYaw(const glm::vec3& cameraPos, const glm::vec3& objectPos, const glm::vec3& up, float& pitch, float& yaw) {
    glm::vec3 direction = glm::normalize(objectPos - cameraPos);

    // Calculate yaw (left/right rotation)
    yaw = glm::degrees(atan2(direction.x, direction.z));

    // Calculate pitch (up/down rotation)
    glm::vec3 right = glm::normalize(glm::cross(up, direction));
    pitch = glm::degrees(atan2(-direction.y, glm::length(right)));
}

void Scene::configureLightProperty(LightProperty& prop)
{
    prop.dirLight.direction = { -0.2f, -1.0f, -0.3f };
    prop.dirLight.ambient = { 0.5f, 0.5f, 0.5f };
    prop.dirLight.diffuse = { 0.4f, 0.4f, 0.4f };
    prop.dirLight.specular = { 0.5f, 0.5f, 0.5f };

    PointLight p1;
    p1.position = spherePosition1;
    p1.ambient = { 0.1f, 0.1f, 0.1f };
    p1.diffuse = { 1.0f, 1.0f, 0.9f };
    p1.specular = { 0.1f, 0.1f, 0.1f };
    p1.constant = 1.0f;
    p1.linear = 0.09f;
    p1.quadratic = 0.032f;
    prop.pointLights.push_back(p1);

    PointLight p2;
    p2.position = spherePosition2;
    p2.ambient = { 0.1f, 0.1f, 0.1f };
    p2.diffuse = { 1.0f, 1.0f, 0.9f };
    p2.specular = { 0.1f, 0.1f, 0.1f };
    p2.constant = 1.0f;
    p2.linear = 0.09f;
    p2.quadratic = 0.032f;
    prop.pointLights.push_back(p2);

    SpotLight s1;
    s1.position = spotLightPosition1;
    s1.initialPosition = spotLightPosition1;
    s1.pitch = -45.0f;
    s1.yaw = -45.0f;
    s1.ambient = { 0.1f, 0.1f, 0.1f };
    s1.diffuse = { 1.0f, 1.0f, 1.0f };
    s1.specular = { 1.0f, 1.0f, 1.0f };
    s1.constant = 0.1f;
    s1.linear = 0.01f;
    s1.quadratic = 0.001f;
    s1.cutOff = glm::cos(glm::radians(10.0f));
    s1.outerCutOff = glm::cos(glm::radians(20.0f));
    prop.spotLights.push_back(s1);
    // Instead of setting direction, calculate it with pitch and yaw.
    prop.processMovement(U, 0);
}


void Scene::run()
{
    Shader::addCommonFile("res\\shaders\\light.glsl");
    // build and compile shaders
    Shader objectShader("res\\shaders\\object.vs", "res\\shaders\\object.fs");
    Shader sphereShader("res\\shaders\\sphere.vs", "res\\shaders\\sphere.fs");

    // load models
    Model boardModel("res/board/board.obj");
    Model whiteKingModel("res/king/king.obj");
    Model knightModel("res/knight/knight.obj");
    Model sphereModel("res/sphere/sphere.obj");


    // TODO: IluminatedObjects should be in vector
    // TODO: Objects should have some reset function to move them to 0.0 point
    Board board(objectShader, boardModel);
    WhiteKing whiteKing(objectShader, whiteKingModel);
    Knight knight(objectShader, knightModel);
    Sphere sphere1(sphereShader, sphereModel, spherePosition1);
    Sphere sphere2(sphereShader, sphereModel, spherePosition2);


    LightProperty lightProperty;
    configureLightProperty(lightProperty);

    ConditionsController conditionsController;

    camera.SetNewPosition(staticCameraPos, staticCameraPitch, staticCameraYaw);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, conditionsController, lightProperty);
        //std::cout << camera.Position.x <<
        //    " " << camera.Position.y << 
        //    " "<<camera.Position.z << endl;

        //std::cout << "Pitch: " << camera.Pitch << endl;
        //std::cout << "Yaw: " << camera.Yaw<< endl;
        conditionsController.updateTime();
        lightProperty.updateLight(conditionsController, whiteKing.getOffset());

        auto background = conditionsController.getBackgroundColor();
        glClearColor(background.r, background.g, background.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (cameraMode == Tracking)
        {
            camera.SetNewPosition(trackingCameraPos,
                trackingCameraBasePitch + whiteKing.getPosition() * trackingCameraExtraPitch,
                trackingCameraBaseYaw + whiteKing.getPosition() * trackingCameraExtraYaw);
        }

        if (cameraMode == POV)
        {
            camera.SetNewPosition(POVCameraPos - glm::vec3(0.0f, 0.0f, whiteKing.getOffset()), camera.Pitch, camera.Yaw);
        }

        board.draw(lightProperty, camera, conditionsController);
        whiteKing.draw(lightProperty, camera, conditionsController);
        whiteKing.move(deltaTime);
        knight.draw(lightProperty, camera, conditionsController);
        sphere1.draw(lightProperty, camera, conditionsController);
        sphere2.draw(lightProperty, camera, conditionsController);

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

void Scene::changeCamera()
{
    if (cameraMode == Static) cameraMode = POV;
    else if (cameraMode == POV) cameraMode = Tracking;
    else if (cameraMode == Tracking) cameraMode = Free;
    else cameraMode = Static;

    if (cameraMode == Free)
        camera.positionFixed = false;
    else
        camera.positionFixed = true;

    if (cameraMode == Free || cameraMode == POV)
        camera.mouseFixed = false;
    else
        camera.mouseFixed = true;

    if (cameraMode == Static)
    {
        camera.SetNewPosition(staticCameraPos, staticCameraPitch, staticCameraYaw);
    }
}

void Scene::processInput(GLFWwindow* window, ConditionsController &controller, LightProperty &lightProperty)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 1 - fog
    // 2 - camera
    // 3 - shaking
    // 4 - lights
    // 5 - time

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !wasPressed)
    {
        controller.changeFog();
        wasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !wasPressed)
    {
        changeCamera();
        wasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !wasPressed)
    {
        controller.objectShaking = !controller.objectShaking;
        wasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS && !wasPressed)
    {
        controller.lightsOn = !controller.lightsOn;
        wasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS && !wasPressed)
    {
        controller.timeStop = !controller.timeStop;
        wasPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_2) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_4) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE)
            wasPressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        lightProperty.processMovement(U, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        lightProperty.processMovement(L, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        lightProperty.processMovement(R, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        lightProperty.processMovement(D, deltaTime);
}
