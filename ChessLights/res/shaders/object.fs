#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

in vec3 GouradColor;
flat in vec3 FlatGouradColor;

uniform vec3 viewPos;
uniform int shadeMode;

vec3 calcColorWithLight(vec3 fragPos, vec3 normal, vec2 texCoord, vec3 viewPos);
vec3 addFog(vec3 color, float distanceFromCamera);

void main()
{
    vec3 result;
    if (shadeMode == 0)
        result = calcColorWithLight(FragPos, Normal, TexCoords, viewPos);
    else if (shadeMode == 1)
        result = GouradColor;
    else
        result = FlatGouradColor;
    result = addFog(result, length(FragPos - viewPos));
    FragColor = vec4(result, 1.0f);
}

