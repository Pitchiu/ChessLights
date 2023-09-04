#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform bool sphereOn;
//vec3 calcColorWithLight(vec3 FragPos, vec3 Normal, vec3 viewPos);
vec3 addFog(vec3 color, float distanceFromCamera);

void main()
{

    vec3 result;
    if (sphereOn) 
        result = vec3(1.0f, 1.0f, 1.0f);
    else
        result = vec3(0.2f, 0.2f, 0.2f);
    result = addFog(result, length(FragPos - viewPos));
    FragColor = vec4(result, 1.0f);
} 

