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

const vec2 square[6] = {
	vec2(100, 100),
	vec2(100, -100),
	vec2(-100, -100),
	vec2(-100, -100),
	vec2(-100, 100),
	vec2(100, 100)
};

const vec2 texCoord[6] = {
	vec2(1.0, 1.0),
	vec2(1.0, 0.0),
	vec2(0.0, 0.0),
	vec2(0.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
};

struct Fog{
    vec4 color;
    vec2 position;
};
layout(location = 6) uniform Fog F;

//layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 texture_coordinate;

layout(location = 0) out vec3 fs_position;
layout(location = 1) out vec3 fs_normal;
layout(location = 2) out vec2 fs_texture_coordinate;

void main()
{
	if( gl_VertexID < 6){
		fs_position = vec3(square[gl_VertexID].x, -1.37f, square[gl_VertexID].y);
		fs_texture_coordinate = texCoord[gl_VertexID] * 50;
	}
	else{
		fs_position = vec3(square[gl_VertexID-6].y, square[gl_VertexID-6].x, -100.0);
		fs_texture_coordinate = texCoord[gl_VertexID-6] * 50;
	}
	fs_normal = vec3(1,0,0);
	fs_texture_coordinate = texCoord[gl_VertexID] * 50;
    vec4 MVposition = camera.view * object.model_matrix * vec4(fs_position, 1.0);
    gl_Position = camera.projection * MVposition;
}
