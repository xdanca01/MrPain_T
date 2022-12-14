#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
// There are no input variables.

// ----------------------------------------------------------------------------
// Output Variables
layout (location = 0) out vec4 final_color;
// ----------------------------------------------------------------------------
// There are no output variables.
uniform bool broom = false;
// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	if(broom){
		final_color = vec4(vec3(0.0), 1.0);
	}
	// We do nothing, but the depth is still processed and stored by default.
}