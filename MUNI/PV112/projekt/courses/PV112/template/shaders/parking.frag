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

layout(binding = 3) uniform sampler2D albedo_texture;
layout(binding = 4) uniform sampler2D normal_texture;
layout(location = 5) uniform bool fog_enable = false;
layout(location = 8) uniform bool phong_enable = true;
layout(location = 9) uniform bool toon_enable = false;
layout(location = 10) uniform bool has_cone = false;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;
layout(location = 3) in mat3 TBN;

layout(location = 0) out vec4 final_color;

/*vec3 procedural_texture(vec2 uv){
    float r = sin(uv.x*2000.73 + uv.y*1000.4);
    if(r < 0.1) r = 0.13 + uv.x - uv.y;
    return vec3(r, r, r);
}*/

float fogCalc(float z){
    float LOG2 = 1.442695;
    float density_square = F.color.w * F.color.w;
    float distance_square = z * z;
    float fogFact = exp2(-density_square * distance_square * LOG2);
    return clamp(fogFact, 0.0, 1.0);
}

void main() {
    vec3 color_sum = vec3(0.0);
    vec3 normal = normalize(fs_normal);
    normal = texture(normal_texture, fs_texture_coordinate).rgb;
    normal = normal * 2.0 - 1.0;
    normal = -normalize(TBN * normal);
    //phong
    if(phong_enable){
        for(int i = 0; i < lights.length()-2; ++i)
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
    }
    //toon shading
    if(toon_enable){
        //diffuse color, if there is no color;
        if(!phong_enable){
            color_sum = object.diffuse_color.rgb;//texture(normal_texture, fs_texture_coordinate).rgb;//vec3(1.0, 1.0, 1.0);
        }
        vec3 lightDir = normalize(vec3(-1.5, 1.0, -1.0));
        float intesity = dot(lightDir, normal);
        if(intesity < 0.2){
            color_sum = color_sum * 0.1;
        }
        else if(intesity < 0.4){
            color_sum = color_sum * 0.3;
        }
        else if(intesity < 0.6){
            color_sum = color_sum * 0.5;
        }
        else if(intesity < 0.8){
            color_sum = color_sum * 0.7;
        }
        else{
            color_sum = color_sum * 0.9;
        }
    }
    //Cone lightning
    if(has_cone && fs_position.y > 0.4){
        float spotExp = 0.6;
        vec3 spotDirection = vec3(0.0, -0.1, 1.0);
        Light L1 = lights[lights.length()-2];
        Light L2 = lights[lights.length()-1];
        vec3 lightDir1 = normalize(fs_position - L1.position.xyz);
        vec3 lightDir2 = normalize(fs_position - L2.position.xyz);
        float theta1 = max(dot(spotDirection, lightDir1), 0.0);
        float theta2 = max(dot(spotDirection, lightDir2), 0.0);
        float checkSpot1 = dot(normalize(normal), -lightDir1);
        float checkSpot2 = dot(normalize(normal), -lightDir2);
        if(theta1 > spotExp && L1.diffuse_color.b > 0.0 && checkSpot1 > -0.01){
            color_sum = mix(color_sum, L1.diffuse_color.rgb, 0.2);
        }
        if(theta2 > spotExp && L2.diffuse_color.b > 0.0 && checkSpot2 > -0.01){
            color_sum = mix(color_sum, L2.diffuse_color.rgb, 0.2);
        }
    }
    //fog
    if(fog_enable){
        float Z = length(fs_position - camera.position);
        float fogFact = fogCalc(Z);
        color_sum = mix(F.color.rgb, color_sum, fogFact);
    }
    color_sum = color_sum / (color_sum + 1.0);   // tone mapping
    color_sum = pow(color_sum, vec3(1.0 / 2.2)); // gamma correction
    final_color = vec4(color_sum, 1.0);
    //final_color = vec4(procedural_texture(fs_texture_coordinate), 1.0);
}
