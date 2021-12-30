#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 mvp;
uniform float time;

out vec2 uv;
out vec4 vertex_position_worldspace;

void main()
{
    vertex_position_worldspace = mvp * vec4(vertex_position * clamp(time, 1, 5), 1.0);
    gl_Position = vertex_position_worldspace;

    uv = vertex_uv; 
}
