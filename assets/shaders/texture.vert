#version 330 core

layout(location = 0) in vec3 vertex_position_modelspace;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

out vec2 uv;
out vec3 normal;
out vec4 vertex_position_worldspace;

uniform mat4 mvp;

void main()
{
    vertex_position_worldspace = mvp * vec4(vertex_position_modelspace, 1.0);
    gl_Position = vertex_position_worldspace;

    uv = vertex_uv;
    normal = vertex_normal;
}
