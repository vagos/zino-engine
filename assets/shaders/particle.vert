#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 mvp;

out vec2 uv;
out vec4 vertex_position_worldspace;

void main()
{
    uv = vertex_uv;
    vertex_position_worldspace = mvp * vec4(vertex_position, 1.0);
    gl_Position = vertex_position_worldspace;
}
