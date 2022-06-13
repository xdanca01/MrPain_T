#version 450

layout(binding = 0, std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
}
camera;

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 1, std430) buffer Lights{
	Light lights[];
};

layout(binding = 2, std140) uniform Object {
    mat4 model_matrix;

    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
}
object;

struct Fog{
    vec4 color;
    vec2 position;
};
layout(location = 6) uniform Fog F;

layout(location = 3) uniform bool has_texture = false;
layout(location = 5) uniform bool fog_enable = false;

layout(binding = 3) uniform sampler2D albedo_texture;
layout(binding = 4) uniform sampler2D normal_texture;


layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 final_color;

float fogCalc(float z){
    float LOG2 = 1.442695;
    float density_square = F.color.w * F.color.w;
    float distance_square = z * z;
    float fogFact = exp2(-density_square * distance_square * LOG2);
    return clamp(fogFact, 0.0, 1.0);
}

void main() {
    vec3 color_sum = vec3(0.0);
    //vec3 normal = fs_normal;
    vec3 normal = normalize(texture(normal_texture, fs_texture_coordinate).rgb * 2.0 - 1.0);
    for(int i = 0; i < lights.length(); ++i)
    {
        Light light = lights[i];
        vec3 light_vector = lights[i].position.xyz - fs_position * lights[i].position.w;
        vec3 L = normalize(light_vector);
        vec3 N = normalize(normal);
        vec3 E = normalize(camera.position - fs_position);
        vec3 H = normalize(L + E);

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0001);

        vec3 ambient = object.ambient_color.rgb * lights[i].ambient_color.rgb;
        vec3 diffuse = object.diffuse_color.rgb * lights[i].diffuse_color.rgb * texture(albedo_texture, fs_texture_coordinate).rgb;
        vec3 specular = object.specular_color.rgb * lights[i].specular_color.rgb;

        vec3 color = ambient.rgb + NdotL * diffuse.rgb + pow(NdotH, object.specular_color.w) * specular;
        color /= (length(light_vector * light_vector));

        color_sum += color;
    }
    if(fog_enable){
        float Z = length(fs_position - camera.position);
        float fogFact = fogCalc(Z);
        color_sum = mix(F.color.rgb, color_sum, fogFact);
    }
    color_sum = color_sum / (color_sum + 1.0);   // tone mapping
    color_sum = pow(color_sum, vec3(1.0 / 2.2)); // gamma correction
    final_color = vec4(color_sum, 1.0);
}
