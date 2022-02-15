#version 330 core

layout(location = 0) in vec3 vertex_position_first;
layout(location = 1) in vec3 vertex_position_second;

uniform float morph_factor;
uniform mat4 mvp;

out vec4 color;

void main()
{
      gl_Position = mvp * vec4(vertex_position_first * (1 - morph_factor) 
              + vertex_position_second * morph_factor, 1);
}
