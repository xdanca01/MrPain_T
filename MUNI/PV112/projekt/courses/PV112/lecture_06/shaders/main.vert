#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// The projection matrix.
layout(location = 0) uniform mat4 projection;
// The model matrix.
layout(location = 1) uniform mat4 model;
// The view matrix.
layout(location = 2) uniform mat4 view;

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The normal of the current vertex that is being processed.
layout(location = 1) in vec3 normal;
// The texture coordinates of the current vertex that is being processed.
layout(location = 2) in vec2 texture_coordinate;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The position forwarded to fragment shader.
layout(location = 0) out vec3 fs_position;
// The normal forwarded to fragment shader.
layout(location = 1) out vec3 fs_normal;
// The texture coordinates forwarded to fragment shader.
layout(location = 2) out vec2 fs_texture_coordinate;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	fs_position = vec3(model * vec4(position, 1.0));
	fs_normal = transpose(inverse(mat3(model))) * normal;
	fs_texture_coordinate = texture_coordinate;

    gl_Position = projection * view * model * vec4(position, 1.0);
}
