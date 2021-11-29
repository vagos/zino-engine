#version 330 core

layout (location = 0) in vec3 vertex_position_modelspace;

out vec3 uv;

uniform mat4 mvp;

void main()
{
    vec4 vertex_position_worldspace = mvp * vec4(vertex_position_modelspace, 1.0);
    gl_Position = vertex_position_worldspace.xyww;

    uv = vertex_position_modelspace * 2;
}
