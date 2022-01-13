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
uniform sampler2D texture_sampler_overlay;

uniform s_material material;
uniform s_light light;

uniform float time;

void main()
{
    color = vec4(
        mix(texture(texture_sampler, uv).rgb, 
            texture(texture_sampler_overlay, uv + time / 20.0).rgb, 0.5)
        ,1.0);
}
