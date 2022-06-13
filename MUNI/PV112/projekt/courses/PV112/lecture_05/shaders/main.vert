#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// Task 5.1: Add uniform variables for projection, view, model matrices.
layout(location = 0) uniform mat4 projection;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;
// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The normal of the current vertex that is being processed.
layout(location = 1) in vec3 normal;
// The texture coordinates of the current vertex that is being processed.
layout(location = 2) in vec2 texture_coordinate;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The position forwared to fragment shader.
layout(location = 0) out vec3 fs_position;
// The normal forwared to fragment shader.
layout(location = 1) out vec3 fs_normal;
// The texture coordinates forwared to fragment shader.
layout(location = 2) out vec2 fs_texture_coordinate;
// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Task 5.1 Transform position to world space when you obtain model matrix.
	fs_position = vec3(model * vec4(position, 1.0f));
	// Task 5.1 Uncomment when you obtain model matrix.
	fs_normal = transpose(inverse(mat3(model))) * normal;
	fs_texture_coordinate = texture_coordinate;
	// Task 5.1: Multiply position by MVP matrices in correct order 
	gl_Position = projection* view* model* vec4(position, 1.0);
}
