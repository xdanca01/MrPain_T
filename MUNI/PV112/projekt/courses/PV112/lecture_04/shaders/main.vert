#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The color of the current vertex that is being processed.
layout(location = 1) in vec4 color;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The color forwared to fragment shader.
layout(location = 0) out vec4 out_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	out_color = color;
    gl_Position = vec4(position, 1.0);
}
