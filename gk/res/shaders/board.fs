#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

vec3 calculateColorWithLight(vec3 FragPos, vec3 Normal, vec3 viewPos);

void main()
{
    vec3 result = calculateColorWithLight(FragPos, Normal, viewPos);
    FragColor = vec4(result, 1.0f);
} 

