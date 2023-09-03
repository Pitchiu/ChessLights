#pragma once

#include "shader_m.h"
#include "model.h"
#include "camera.h"


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
};

struct LightProperty
{
    DirLight dirLight;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
};

class IluminatedObject
{
public:
	Shader& shader;
	Model& model;

	IluminatedObject(Shader& shader, Model& model);
	void configureIlumination(const LightProperty&);
    virtual void draw(const LightProperty& prop, const Camera& camera);
};

class WhiteKing : IluminatedObject
{
public:
    WhiteKing(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera) override;
};

class Board : IluminatedObject
{
public:
    Board(Shader& shader, Model& model);
    void draw(const LightProperty& prop, const Camera& camera) override;
};
