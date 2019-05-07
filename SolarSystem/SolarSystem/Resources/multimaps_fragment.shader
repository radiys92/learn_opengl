#version 330 core
in vec2 TexCoord;
in vec2 CloudsTexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D mainTex;
uniform sampler2D cloudsTex;
uniform sampler2D lightsTex;
uniform sampler2D specularTex;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularMap = texture(specularTex, TexCoord).r;
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
    vec3 specular = (spec * lightColor);
    specular = specular * specularStrength + specular * (1 - specularMap) * 2;


    vec3 self = texture(mainTex, TexCoord).rgb;
    self += texture(cloudsTex, CloudsTexCoord).r / 2;

    vec3 lights = texture(lightsTex, TexCoord).rgb;

    vec3 result = ambient + diffuse + specular;
    result = result * self + (1 - result) * lights;
    color = vec4(result, 1.0f);
} 