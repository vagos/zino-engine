#version 330 core

// This shader handles ligthing!

struct s_material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct s_light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Kc;
    float Kl;
    float Kq;
};

in vec4 vertex_position_worldspace;
in vec3 normal;
in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;
uniform vec3 light_position;
uniform vec3 view_position;
uniform s_material material;
uniform s_light light;

void main()
{

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light.position - vertex_position_worldspace.xyz);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 view_direction = normalize(view_position - vertex_position_worldspace.xyz);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // slowly fade off light
    float d = distance(light.position, vertex_position_worldspace.xyz);
    float attenuation = 1.0 / (light.Kc + light.Kl * d + light.Kq * d * d);
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   

    // color = vec4( (ambient + diffuse + specular) * texture(texture_sampler, uv).rgb, 1.0);
    color = vec4( texture(texture_sampler, uv).rgb, 1.0);
}
