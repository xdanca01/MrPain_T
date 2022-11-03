#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec2 tex_coord;  // The vertex texture coordinates.
} in_data;

// The texture to be blurred.
layout (binding = 0) uniform sampler2D highlights_texture;


// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// The final output color.
layout (location = 0) out vec4 final_blurred_highlight;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// TASK 4: Blur input texture. 
	//    Hint: Usefull functions: textureSize, textureLod, max
	//    Hint: We want to spread bright pixel = use max function.
	//	  Hint: What about kernel size? try starting with 5x5
	vec3 H = vec3(0.0);
	vec2 texelSize = 1.0 / textureSize(highlights_texture, 0);
	//
	for(int i = -2; i < 2; ++i){
		for(int j = -2; j < 2; ++j){
			vec2 cords = vec2(in_data.tex_coord.x + j*texelSize.x, in_data.tex_coord.y + i*texelSize.y);
			H = H + texture(highlights_texture, cords).rgb * textureLod(highlights_texture, cords, 0).rgb;
		}
	}
	int div = 25;
	final_blurred_highlight = vec4(max(H/div, textureLod(highlights_texture, in_data.tex_coord, 0).rgb), 1.0);
}