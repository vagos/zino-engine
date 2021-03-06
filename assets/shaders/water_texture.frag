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

// uniforms

uniform sampler2D texture_sampler;

uniform s_material material;
uniform s_light light;

uniform float time;

void main()
{
    vec3 extra_color = vec3(0.294, 0, ( clamp(sin(time), 0.509, 1.0) ) );

    color = vec4( extra_color * texture(texture_sampler, uv * time / 20.0f).rgb, 1.0);
}
