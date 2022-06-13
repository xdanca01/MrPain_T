#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// Task 3.8: Define the input variable for color.

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) in vec3 fs_color;
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
    // Task 3.8: Use the color from the vertex shader and set it to the final color.
    final_color = vec4(fs_color, 1.0);
}
