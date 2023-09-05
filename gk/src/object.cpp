#include "object.h"
#include "random"
#include "scene.h"

IluminatedObject::IluminatedObject(Shader& shader, Model& model) : shader(shader), model(model)
{
}

void IluminatedObject::configureIlumination(const LightProperty& prop)
{
    shader.setVec3("dirLight.direction", prop.dirLight.direction);
    shader.setVec3("dirLight.ambient", prop.dirLight.ambient);
    shader.setVec3("dirLight.diffuse", prop.dirLight.diffuse);
    shader.setVec3("dirLight.specular", prop.dirLight.specular);
    // pointLight
    for (int i = 0; i<prop.pointLights.size(); i++)
    {
        shader.setVec3("pointLights[" + to_string(i) + "].position", prop.pointLights[i].position);
        shader.setVec3("pointLights[" + to_string(i) + "].ambient", prop.pointLights[i].ambient);
        shader.setVec3("pointLights[" + to_string(i) + "].diffuse", prop.pointLights[i].diffuse);
        shader.setVec3("pointLights[" + to_string(i) + "].specular", prop.pointLights[i].specular);
        shader.setFloat("pointLights[" + to_string(i) + "].constant", prop.pointLights[i].constant);
        shader.setFloat("pointLights[" + to_string(i) + "].linear", prop.pointLights[i].linear);
        shader.setFloat("pointLights[" + to_string(i) + "].quadratic", prop.pointLights[i].quadratic);
    }

    //SpotLight s1;
    //s1.position = spotLight;
    //s1.direction = { -0.8f, -0.8f, -0.8f };
    //s1.ambient = { 0.1f, 0.1f, 0.1f };
    //s1.diffuse = { 1.0f, 1.0f, 1.0f };
    //s1.specular = { 1.0f, 1.0f, 1.0f };
    //s1.constant = 0.1f;
    //s1.linear = 0.01f;
    //s1.quadratic = 0.001f;
    //s1.cutOff = glm::radians(30.0f);
    //s1.outerCutOff = glm::radians(45.0f);
    //prop.pointLights.push_back(p2);

    for (int i = 0; i < prop.spotLights.size(); i++)
    {
        shader.setVec3("spotLights[" + to_string(i) + "].position", prop.spotLights[i].position);
        shader.setVec3("spotLights[" + to_string(i) + "].direction", prop.spotLights[i].direction);
        shader.setVec3("spotLights[" + to_string(i) + "].ambient", prop.spotLights[i].ambient);
        shader.setVec3("spotLights[" + to_string(i) + "].diffuse", prop.spotLights[i].diffuse);
        shader.setVec3("spotLights[" + to_string(i) + "].specular", prop.spotLights[i].specular);
        shader.setFloat("spotLights[" + to_string(i) + "].constant", prop.spotLights[i].constant);
        shader.setFloat("spotLights[" + to_string(i) + "].linear", prop.spotLights[i].linear);
        shader.setFloat("spotLights[" + to_string(i) + "].quadratic", prop.spotLights[i].quadratic);
        shader.setFloat("spotLights[" + to_string(i) + "].cutOff", prop.spotLights[i].cutOff);
        shader.setFloat("spotLights[" + to_string(i) + "].outerCutOff", prop.spotLights[i].outerCutOff);
    }
}

void IluminatedObject::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    configureIlumination(prop);
    shader.setMat4("projection", camera.GetProjectionMatrix());
    shader.setMat4("view", camera.GetViewMatrix());
    shader.setVec3("viewPos", camera.Position);

    // fog
    shader.setVec3("skyColor", conditionsController.getBackgroundColor());
    shader.setFloat("fogDensity", conditionsController.getFogDensity());
    shader.setBool("lightsOn", conditionsController.lightsOn);

    shader.setInt("shadeMode", conditionsController.shadeMode);

    model.Draw(shader);
}

WhiteKing::WhiteKing(Shader& shader, Model& model) : IluminatedObject(shader, model)
{
}


float getRandomFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    return distribution(gen);
}

void WhiteKing::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);

    // third - move
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -offset));

    // second - translate to 0.0, rotate, and translate back
    model = glm::translate(model, glm::vec3(1.0f, 0.0f, 8.0f));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    if (conditionsController.objectShaking)
    {
        model = glm::rotate(model, glm::radians(getRandomFloat()), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(getRandomFloat()), glm::vec3(0.0f, 0.0f, 1.0f));
    }

    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -8.0f));

    // first - set initial position
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

    shader.setMat4("model", model);

    shader.setVec3("material.specular", 0.54f, 0.54f, 0.54f);
    shader.setFloat("material.shininess", 36.0f);

    IluminatedObject::draw(prop, camera, conditionsController);
}

void WhiteKing::move(float deltaTime)
{
    if (stop) return;

    float velocity = speed * deltaTime;
    angle += velocity*angleSpeed;
    if (angle > 360.0f)
        angle -= 360.0f;
    if (direction == Forward)
        offset += velocity;
    else
        offset -= velocity;

    if (offset > maxDeflection)
    {
        offset = maxDeflection;
        direction = Backward;
    }
    else if (offset < 0.0f)
    {
        offset = 0.0f;
        direction = Forward;
    }
}

float WhiteKing::getPosition()
{
    return offset / maxDeflection;
}

float WhiteKing::getOffset()
{
    return offset;
}

Board::Board(Shader& shader, Model& model) : IluminatedObject(shader, model)
{
}

void Board::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    shader.setMat4("model", model);

    shader.setVec3("material.specular", 0.0f, 0.0f, 0.0f);
    shader.setFloat("material.shininess", 10.0f);

    IluminatedObject::draw(prop, camera, conditionsController);
}

Knight::Knight(Shader& shader, Model& model) : IluminatedObject(shader, model)
{
}

void Knight::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);

    shader.setVec3("material.specular", 0.54f, 0.54f, 0.54f);
    shader.setFloat("material.shininess", 36.0f);

    IluminatedObject::draw(prop, camera, conditionsController);
}

Sphere::Sphere(Shader& shader, Model& model, glm::vec3 position) : IluminatedObject(shader, model), position(position)
{
}

void Sphere::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

    shader.setMat4("model", model);
    shader.setBool("sphereOn", conditionsController.lightsOn);


    IluminatedObject::draw(prop, camera, conditionsController);
}

Pawn::Pawn(Shader& shader, Model& model) : IluminatedObject(shader, model)
{

}

void Pawn::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);

    shader.setVec3("material.specular", 0.54f, 0.54f, 0.54f);
    shader.setFloat("material.shininess", 36.0f);

    IluminatedObject::draw(prop, camera, conditionsController);
}

Rook::Rook(Shader& shader, Model& model) : IluminatedObject(shader, model)
{

}

void Rook::draw(const LightProperty& prop, const Camera& camera, const ConditionsController& conditionsController)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 5.0f));
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);

    shader.setVec3("material.specular", 0.54f, 0.54f, 0.54f);
    shader.setFloat("material.shininess", 36.0f);

    IluminatedObject::draw(prop, camera, conditionsController);
}