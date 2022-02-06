#version 330 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out vec4 vertex_position_worldspace;

uniform mat4 mvp[100];
uniform float time;

float rand(vec2 co){
    return fract((dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    mat4 mvp_i = mvp[gl_InstanceID];
    vec4 pos = vec4(vertex_position + vertex_normal * abs(sin(2*time + 3*rand(vertex_uv))) * 2.0 , 1.0);

    vertex_position_worldspace = mvp_i * pos;

    gl_Position = vertex_position_worldspace;

    uv = vertex_uv; 
    normal = vertex_normal;
}
