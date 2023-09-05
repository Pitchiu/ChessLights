#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

out vec3 GouradColor;
flat out vec3 FlatGouradColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;

vec3 calcColorWithLight(vec3 fragPos, vec3 normal, vec2 texCoord, vec3 viewPos);

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;

    GouradColor = calcColorWithLight(aPos, aNormal, aTexCoords, viewPos);
    FlatGouradColor = GouradColor;
}
