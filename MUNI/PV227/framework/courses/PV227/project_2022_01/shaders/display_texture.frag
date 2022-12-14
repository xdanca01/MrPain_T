#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec2 tex_coord;  // The vertex texture coordinates.
} in_data;

uniform float mirror_factor;

// The texture to display.
layout (binding = 0) uniform sampler2D gbuffer_albedo_texture;
layout (binding = 1) uniform sampler2D gbuffer_mask_texture;
layout (binding = 2) uniform sampler2D gbuffer_albedo_texture_mirror;
layout (binding = 3) uniform sampler2D depth_texture;

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
	vec3 mask = texture(gbuffer_mask_texture, in_data.tex_coord).rgb;
	vec3 color = texture(gbuffer_albedo_texture, in_data.tex_coord).rgb;
	vec3 color1 = texture(gbuffer_albedo_texture_mirror, in_data.tex_coord).rgb;
	if(mask == vec3(1.0, 0.0, 0.0) && color1 != vec3(0.0)){
		final_color = vec4(mix(color, color1, mirror_factor), 1.0);
	}
	else{
		final_color = vec4(color, 1.0);
	}
}