#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

layout(binding = 0) uniform sampler2D input_image;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Local Variables
// ----------------------------------------------------------------------------

// Gaussian kernel
const float gaussBlurKernel[3][3] = {
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
	{2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
};

// Edge detection convolution kernel
const float edgeDetectionKernel[3][3] = {
	{-1.0, -1.0, -1.0},
	{-1.0,  8.0, -1.0},
	{-1.0, -1.0, -1.0}
};

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------

void main() {
	vec3 left_px2 = texelFetch(input_image, ivec2(gl_FragCoord.x-2, gl_FragCoord.y), 0).rgb;
	vec3 left_px = texelFetch(input_image, ivec2(gl_FragCoord.x-1, gl_FragCoord.y), 0).rgb;
	vec3 centre_px = texelFetch(input_image, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0).rgb;
	vec3 right_px = texelFetch(input_image, ivec2(gl_FragCoord.x+1, gl_FragCoord.y), 0).rgb;
	vec3 right_px2 = texelFetch(input_image, ivec2(gl_FragCoord.x+2, gl_FragCoord.y), 0).rgb;

	//edge detection v X-ovem smeru
	centre_px += left_px2 + left_px + right_px + right_px2 - centre_px*4;
	
	
	//rozmazani v x-ovem smeru
	//centre_px += left_px2 + left_px + right_px + right_px2 - centre_px*4;

	// --------------------------------------------------------------------------
	// Task 7.3: Apply post-process effects (grayscale, convolution kernels).
	// --------------------------------------------------------------------------
	// Use texelFetch(https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/texelFetch.xhtml)
	// to retrieve exact pixel and gl_FragCoord.xy to retrieve the coordinates of the fragment 
	// currently being processed.
	//
	// - make the color grayscale (https://en.wikipedia.org/wiki/Grayscale)
	// - apply a convolution kernel (https://en.wikipedia.org/wiki/Kernel_(image_processing))

	final_color = vec4(centre_px, 1.0);	
}