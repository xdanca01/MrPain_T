#version 450

// The buffer with data about camera.
layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

// Task 8.2: Replace the single light with SSBO containing dynamic array of lights.
// The buffer with data about light.
struct Light {
    vec4 position;
    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
};

// The buffer with data about all lights.
layout(binding = 1, std430) buffer Lights {
	Light lights[];
};

// Task 8.2: Replace the single object with SSBO containing dynamic array of objects.
// The buffer with data about object.
layout(binding = 2, std140) uniform Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
} object;

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The normal of the current vertex that is being processed.
layout(location = 1) in vec3 normal;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// The position forwarded to fragment shader.
layout(location = 0) out vec3 fs_position;
// The normal forwarded to fragment shader.
layout(location = 1) out vec3 fs_normal;

void main()
{
	// Task 8.4: Obtain the object from buffer using gl_InstanceID.

	fs_position = vec3(object.model_matrix * vec4(position, 1.0));
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	// Task 8.4: Forward the gl_InstanceID to fragment shader.
    gl_Position = camera.projection * camera.view * vec4(fs_position, 1.0);
}
