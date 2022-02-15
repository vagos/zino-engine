#version 330

out vec4 color;

in vec2 uv;
in vec4 vertex_position_worldspace;

uniform sampler2D sprite;
uniform float time;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float life_time = 5.0;

    float t = time/life_time;

    // vec3 c = vec3(0.662, 0.635, 0.635 + rand(vec2(t)));
    vec3 c = vec3(0.662, 0.635, 0.635);

    float alpha = (1.0 - t);

    color = vec4(c, alpha);
}
