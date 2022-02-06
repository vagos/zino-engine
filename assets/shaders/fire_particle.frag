#version 330

out vec4 color;

in vec2 uv;
in vec4 vertex_position_worldspace;

uniform sampler2D texture_sampler;
uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float life_time = 5;

    float t = time/life_time;

    vec3 c = vec3((0.5 + time), 0.4 + sin(time), 0) * (1 - t);

    float alpha = 0.7 * (1 - t/3);

    color = vec4(c, 1.0);
}

