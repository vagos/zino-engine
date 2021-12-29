#version 330

out vec4 color;

in vec2 uv;
in vec4 vertex_position_worldspace;

uniform sampler2D sprite;

void main()
{
    vec3 c = vec3(
            abs(vertex_position_worldspace.x), 
            abs(vertex_position_worldspace.y), 
            abs(vertex_position_worldspace.z)) / 100;

    color = vec4(c.xyz, 1.0);
}
