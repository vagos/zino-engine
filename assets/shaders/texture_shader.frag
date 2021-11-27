#version 330 core

in vec4 vertex_position_worldspace;
in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;

void main()
{
    color = vec4(texture(texture_sampler, uv).rgb, 1.0);
}
