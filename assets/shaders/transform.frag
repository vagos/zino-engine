#version 330 core

out vec4 color;

uniform float morph_factor;

void main()
{
    color = vec4(1 * (1 - morph_factor), 0, morph_factor, 1);
}
