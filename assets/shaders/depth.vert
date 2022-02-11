#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex_position_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 vp;
uniform mat4 m;

uniform bool instancing;
uniform mat4 p_m[100];

void main()
{
    mat4 model = m;

    if (instancing)
    {
        model = p_m[gl_InstanceID];
    }

    gl_Position =  vp * model * vec4(vertex_position_modelspace, 1.0);
}
