#version 330 core

// This shader handles ligthing!

struct s_material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct s_light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float Kc;
    float Kl;
    float Kq;
};

in vec4 vertex_position_worldspace;
in vec4 vertex_position_lightspace;
in vec3 normal;
in vec2 uv;

out vec4 color;

uniform sampler2D texture_sampler;
uniform sampler2D shadowmap_sampler;
uniform sampler2D normal_sampler;

uniform s_material material;
uniform s_light light;

uniform mat4 light_vp;
uniform mat4 m;

uniform bool use_extras;

float calculate_shadow(vec4 position_lightspace, sampler2D sm_sampler)
{
   float shadow = 0.0; 

    // Perspective devide to bring coordinates in range[-1, 1]
    vec3 projected_coordinates = position_lightspace.xyz/position_lightspace.w;
    // Since the depth map values are in range[0, 1]
    projected_coordinates = projected_coordinates * 0.5 + 0.5;

    // Sampling the closest point in this position from the depth map
    float closest_depth = texture(sm_sampler, projected_coordinates.xy).r;

    // Then we get the depth of the current vertex
    float current_depth = projected_coordinates.z;

    // Correcting the quantization problem
    float bias = 0.005;
    
    vec2 texel_size = 1.0 / textureSize(sm_sampler, 0);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float depth = texture(sm_sampler, projected_coordinates.xy + vec2(x,y) * texel_size).r;
            shadow += (depth + bias) < current_depth ? 1.0 : 0.0;
        }
    }
    
    shadow /= 9.0;

    // shadow = (closest_depth + bias) < current_depth ? 1.0 : 0.0; // non-smooth shadows

    return shadow;
}

vec3 calculate_light_directional(s_light light, s_material material, float visibility)
{
    vec3 norm = normalize(normal);

    if (use_extras)
    {
        norm = texture(normal_sampler, uv).rgb; 
        norm = normalize(norm);
    }

    vec3 light_direction = normalize(light.position);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 view_direction = normalize(light.position - vertex_position_worldspace.xyz);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    return ambient + (diffuse + specular) * visibility;
}

vec3 calculate_light(s_light light, s_material material, float visibility)
{
    vec3 norm = normalize(normal);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 light_direction = normalize(light.position - vertex_position_worldspace.xyz);
    float diff = max(dot(light_direction, norm), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    vec3 view_direction = normalize(light.position - vertex_position_worldspace.xyz);
    vec3 reflect_direction = reflect(-light_direction, norm);
    float spec = pow(max(dot(view_direction, reflect_direction), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);
    
    // slowly fade off light
    float d = distance(light.position, vertex_position_worldspace.xyz);
    float attenuation = 1.0 / (light.Kc + light.Kl * d + light.Kq * d * d);
    diffuse  *= attenuation;
    specular *= attenuation;   

    return ambient + ( diffuse + specular ) * visibility;
}

void main()
{
    float shadow = calculate_shadow(vertex_position_lightspace, shadowmap_sampler);
    vec3 light_calced = calculate_light_directional(light, material, 1 - shadow); 

    color = vec4( light_calced * texture(texture_sampler, uv).bgr, 1.0);
    // color = vec4(texture(texture_sampler, uv).rgb, 1.0); // no lighting
}
