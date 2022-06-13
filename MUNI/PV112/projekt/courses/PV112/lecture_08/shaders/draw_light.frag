#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
// The intpolated position from the vertex shader.
layout(location = 0) in vec3 fs_position;
// The interpolated color from the vertex shader.
layout(location = 1) in vec3 fs_color;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
    vec3 result = fs_color / (fs_color + 1.0); // tone mapping
    result = pow(result, vec3(1.0 / 2.2));     // gamma correction
	final_color = vec4(result, 1.0);
}
