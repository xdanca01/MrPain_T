#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec2 tex_coord;  // The vertex texture coordinates.
} in_data;


uniform float increase;
uniform bool clear_snow;

// The texture to display.
layout (binding = 0) uniform sampler2D previousSnow;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// The final output color.
layout (location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// We applie the transformation to the color from the texture using the uniform matrix.
	vec3 color = texture(previousSnow, in_data.tex_coord).rgb + vec3(increase);
	color.r = min(color.r, 1.0f);
	if(clear_snow){
		color = vec3(0.0);
	}
	final_color = vec4(color, 1.0);
}