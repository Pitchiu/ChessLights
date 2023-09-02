#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec2 TexCoords;

struct Material {
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
  
uniform Material material;
uniform Light light;
uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;

void main()
{
    // ambient = light.ambient * texture
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
  	
    // diffuse = light.diffuse (?) * diff * texture
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(texture_diffuse1, TexCoords).rgb);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 