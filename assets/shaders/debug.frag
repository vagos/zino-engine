#version 330

out vec4 color;
in vec2 uv;

uniform sampler2D depthmap_sampler;

void main()
{
    // float depth_value = texture(depthmap_sampler, uv).r;
    // color = vec4(vec3(depth_value), 1.0);
    
    color = vec4(vec3(texture(depthmap_sampler, uv).rgb), 1.0);
}
