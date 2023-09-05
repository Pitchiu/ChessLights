#pragma once

#include "shader_m.h"
#include "model.h"
#include "camera.h"
#include "weather.h"

enum Light_Movement {
    U,
    D,
    L,
    R
};

struct DirLight
{
    glm::vec3 direction = { -0.2f, -1.0f, -0.3f };
    glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
    glm::vec3 diffuse = { 0.4f, 0.4f, 0.4f };
    glm::vec3 specular = { 0.5f, 0.5f, 0.5f };
};
struct PointLight : DirLight
{
    glm::vec3 position = { -10.0f, -10.0f, 10.0f };
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};
struct SpotLight : PointLight
{
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));
    glm::vec3 initialPosition = { -10.0f, -10.0f, 10.0f };
    float yaw = -45.0f;
    float pitch = -45.0f;
};

struct LightProperty
{
    DirLight dirLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;

    const float lightSpeed = 50.0f;

    void updateLight(const ConditionsController& controller, float offset)
    {
        dirLight.ambient = controller.getAmbient();
        dirLight.diffuse = controller.getDiffuse();
        dirLight.specular = controller.getSpecular();

        for (int i = 0; i < spotLights.size(); i++)
            spotLights[i].position = spotLights[i].initialPosition - glm::vec3(0.0f, 0.0f, offset);
    }
    void processMovement(Light_Movement dir, float deltaTime)
    {
        for (int i = 0; i < spotLights.size(); i++)
        {
             if(dir == U)
                spotLights[i].pitch += deltaTime * lightSpeed;
            else if (dir == D)
                spotLights[i].pitch -= deltaTime * lightSpeed;
            else if (dir == R)
                spotLights[i].yaw += deltaTime * lightSpeed;
            else if (dir == L)
                spotLights[i].yaw -= deltaTime * lightSpeed;

            float pitch = spotLights[i].pitch;
            float yaw = spotLights[i].yaw;


            if (spotLights[i].pitch > 89.0f)
                spotLights[i].pitch = 89.0f;
            if (spotLights[i].pitch < -89.0f)
                spotLights[i].pitch = -89.0f;

            glm::vec3 newDir;
            newDir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            newDir.y = sin(glm::radians(pitch));
            newDir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            newDir = glm::normalize(newDir);
            spotLights[i].direction = newDir;
        }
    }
};

class IluminatedObject
{
public:
	Shader& shader;
	Model& model;

	IluminatedObject(Shader& shader, Model& model);
	void configureIlumination(const LightProperty&);
    virtual void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController);
};

class WhiteKing : IluminatedObject
{
private:
    enum Direction
    {
        Forward,
        Backward,
    };
    Direction direction = Forward;
    float const speed = 2.0f;
    float const maxDeflection = 10.0f;
    float const angleSpeed = 15.0f;
    float offset = 0.0f;
    float angle = 0.0f;


public:
    WhiteKing(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
    void move(float deltaTime);
    bool stop = false;
    float getPosition();
    float getOffset();
};

class Board : IluminatedObject
{
public:
    Board(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
};

class Knight : IluminatedObject
{
public:
    Knight(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
};

class Pawn : IluminatedObject
{
public:
    Pawn(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
};

class Rook : IluminatedObject
{
public:
    Rook(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
};

class Sphere : IluminatedObject
{
private:
    glm::vec3 position;
public:
    Sphere(Shader& shader, Model& model, glm::vec3 position);
    void draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController) override;
};