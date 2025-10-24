#version 330 core

in vec3 fragPos;
in vec2 TexCoord;
in vec3 normalVector;
out vec4 fragColor;

struct Material 
{
        sampler2D texture_diffuse0;

        bool hasSpecular;
        sampler2D texture_specular0;

        bool hasEmission;
        sampler2D texture_emission0;

        float shininess;
};

struct dirLight
{
        vec3 direction;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

struct PointLight 
{
        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};


uniform vec3 cameraPosition;
uniform Material material;
uniform dirLight directionalLight;
uniform PointLight pointLight[100];
uniform int numPointLights;


vec3 calculateDirectionalLight(dirLight light, vec3 normal, vec3 viewDir);
vec3 calculatePointLights(PointLight light, vec3 normal, vec3 viewDir);


void main()
{
        vec3 normal = normalize(normalVector);
        vec3 viewDir = normalize(cameraPosition - fragPos);

        vec3 result = calculateDirectionalLight(directionalLight, normal, viewDir);

        for(int i=0; i<numPointLights; ++i)
        {
                result += calculatePointLights(pointLight[i], normal, viewDir);
        }


        fragColor = vec4(result, 1.0f);
        /* fragColor = texture(material.texture_diffuse0, TexCoord); */
}


vec3 calculateDirectionalLight(dirLight light, vec3 normal, vec3 viewDir)
{
        vec3 lightDir = normalize(-light.direction);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        //blinn-phong
        vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoord));
        
        float diff = max(dot(normal, lightDir), 0.0f);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoord));

        vec3 specular = vec3(0.0f);
        if(material.hasSpecular)
        {
                float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);
                specular = light.specular * spec * vec3(texture(material.texture_specular0, TexCoord));
        }
        vec3 emission = vec3(0.0f);
        if(material.hasEmission)
        {
                emission = texture(material.texture_emission0, TexCoord).rgb;
        }


        return (ambient + diffuse + specular + emission);
}

vec3 calculatePointLights(PointLight light, vec3 normal, vec3 viewDir)
{
        vec3 lightDir = normalize(light.position - fragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        
        //blinn-phong
        float diff = max(dot(normal, lightDir), 0.0f);
        float spec = pow(max(dot(normal, halfwayDir), 0.0f), material.shininess);

        vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, TexCoord));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse0, TexCoord));
        vec3 specular = vec3(0.0f);
        if(material.hasSpecular)
        {
                specular = light.specular * spec * vec3(texture(material.texture_specular0, TexCoord));
        }
        vec3 emission = vec3(0.0f);
        if(material.hasEmission)
        {
                emission = texture(material.texture_emission0, TexCoord).rgb;
        }


        float attenuation = 1 / (length(light.position - fragPos));
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

       
        return (ambient + diffuse + specular + emission);
}
