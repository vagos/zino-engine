#version 330 core

in vec4 vertex_position_worldspace;
in vec3 normal;
in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;
uniform vec3 light_position;
uniform vec3 view_position;

void main()
{
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - vertex_position_worldspace.xyz);

    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * light_color;

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    float specular_strength = 0.5;
    vec3 view_direction = normalize(view_position - vertex_position_worldspace.xyz);
    vec3 reflect_direction = reflect(-light_direction, norm);
    
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
    vec3 specular = specular_strength * spec * light_color;

    color = vec4( (ambient + diffuse + specular) * normal, 1.0);
}
