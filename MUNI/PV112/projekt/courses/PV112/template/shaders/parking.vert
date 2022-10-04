#version 450

layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

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
} object;

struct Fog{
    vec4 color;
    vec2 position;
};
layout(location = 6) uniform Fog F;

layout(binding = 3) uniform sampler2D albedo_texture;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;
layout(location = 3) out mat3 TBN;

void main()
{
	fs_position = vec3(object.model_matrix * vec4(position, 1.0));
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	fs_texture_coordinate = texture_coordinate * 7;
    vec4 MVposition = camera.view * object.model_matrix * vec4(position, 1.0);
    gl_Position = camera.projection * MVposition;

	vec3 T = normalize(vec3(object.model_matrix * vec4(tangent,   0.0)));
   	vec3 B = normalize(vec3(object.model_matrix * vec4(bitangent, 0.0)));
   	vec3 N = normalize(vec3(object.model_matrix * vec4(normal, 	  0.0)));
	B = cross(N, T);
   	TBN = mat3(T, B, N);

}
