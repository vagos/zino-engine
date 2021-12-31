#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out vec4 vertex_position_worldspace;

uniform float time;
uniform mat4 mvp;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{

    vec3 pos = vertex_position_modelspace + vertex_normal * clamp(sin(time + vertex_uv.x * vertex_uv.y), 0, 0.1); // kind of looks like a robot

    vertex_position_worldspace = mvp * vec4(pos, 1.0);
    gl_Position = vertex_position_worldspace;

    uv = vertex_uv * ( sin(time/2) * 2 + 1); 
    normal = vertex_normal;
}
