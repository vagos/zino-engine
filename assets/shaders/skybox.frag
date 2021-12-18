#version 330 core

out vec4 color;

in vec3 uv;

uniform samplerCube skybox;
uniform float time;

float triangle_func(in float x, in float A, in float T)
{
   return (A/T) * (T - abs(mod(x, (2*T)) - T) );
}

float get_time_of_day()
{
   float max_time = 5; 
   float current_time = triangle_func(time, 1.0, max_time);
   return current_time;
}

void main()
{
    vec4 texture_color = texture(skybox, vec3(uv.x, -uv.y, uv.z));
    color = mix(texture_color.bgra, texture_color.rgba, 
            clamp(get_time_of_day(), 0.0, 1.0));
}
