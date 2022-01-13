#version 330
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_uv;

uniform mat4 mvp[100];
uniform float time;

out vec2 uv;
out vec4 vertex_position_worldspace;

float rand(vec2 co){
    return fract((dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{

    mat4 mvp_i = mvp[gl_InstanceID];

    vec4 pos = vec4(vertex_position * (time + 2) , 1.0);
    // vec4 pos = vec4(vertex_position + rand(vertex_uv*time) * vertex_normal, 1.0);

    /*
    float K = 1.0;
    float v = 1.0;
    float A = 1.0;
    float u = K * (pos.x - v * time);
    pos.y = A * sin(u);
    */
    // 
    
    vertex_position_worldspace = mvp_i * pos;

    gl_Position = vertex_position_worldspace;

    uv = vertex_uv; 
}
