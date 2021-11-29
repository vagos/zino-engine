#version 330 core

struct s_material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec4 vertex_position_worldspace;
in vec3 normal;
in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;
uniform vec3 light_position;
uniform vec3 view_position;
uniform s_material material;

void main()
{
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - vertex_position_worldspace.xyz);

    // ambient
    vec3 ambient = ambient_strength * light_color;

    // diffuse
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    // specular
    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_position - vertex_position_worldspace.xyz);
    vec3 reflect_direction = reflect(-light_direction, norm);
    
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    color = vec4( (ambient + diffuse + specular) * texture(texture_sampler, uv).rgb, 1.0);
}