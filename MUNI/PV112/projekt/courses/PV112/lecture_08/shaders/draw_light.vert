#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// The buffer with data about camera.
layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

// The representation of one light.
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

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The position forwarded to fragment shader.
layout(location = 0) out vec3 fs_position;
// The color forwarded to fragment shader.
layout(location = 1) out vec3 fs_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	vec3 light_position = lights[gl_InstanceID].position.xyz;

	fs_position = position * 0.5 + light_position;
	fs_color = lights[gl_InstanceID].diffuse_color.rgb;

    gl_Position = camera.projection * camera.view * vec4(fs_position, 1.0);
}
