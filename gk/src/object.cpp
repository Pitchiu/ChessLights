#include "object.h"

IluminatedObject::IluminatedObject(Shader& shader, Model& model) : shader(shader), model(model)
{
}

void IluminatedObject::configureIlumination(const LightProperty& prop)
{
    shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // TODO: set attributes in loop
    // pointLight
    shader.setVec3("pointLights[0].position", -10.0f, -10.0f, 10.0f); //changed
    shader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("pointLights[0].constant", 1.0f);
    shader.setFloat("pointLights[0].linear", 0.09f);
    shader.setFloat("pointLights[0].quadratic", 0.032f);
    // spotLight
    shader.setVec3("spotLight[0].position", 5.0f, 5.0f, 5.0f); //changed
    shader.setVec3("spotLight[0].direction", -0.5f, -0.5f, -0.5f); //changed
    shader.setVec3("spotLight[0].ambient", 0.0f, 0.0f, 0.0f);
    shader.setVec3("spotLight[0].diffuse", 1.0f, 1.0f, 1.0f);
    shader.setVec3("spotLight[0].specular", 1.0f, 1.0f, 1.0f);
    shader.setFloat("spotLight[0].constant", 1.0f);
    shader.setFloat("spotLight[0].linear", 0.09f);
    shader.setFloat("spotLight[0].quadratic", 0.032f);
    shader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));
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

    model.Draw(shader);
}

WhiteKing::WhiteKing(Shader& shader, Model& model) : IluminatedObject(shader, model)
{
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
