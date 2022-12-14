#version 450 core

// The ouput primitive specification.
layout(vertices = 3) out;

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec3 position_ws;	  // The vertex position in world space.
	vec3 normal_ws;		  // The vertex normal in world space.
	vec2 tex_coord;		  // The vertex texture coordinates.
} in_data[];

// The UBO with camera data.	
layout (std140, binding = 0) uniform CameraBuffer
{
	mat4 projection;		// The projection matrix.
	mat4 projection_inv;	// The inverse of the projection matrix.
	mat4 view;				// The view matrix
	mat4 view_inv;			// The inverse of the view matrix.
	mat3 view_it;			// The inverse of the transpose of the top-left part 3x3 of the view matrix
	vec3 eye_position;		// The position of the eye in world space.
};

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
out VertexData
{
	vec3 position_ws;	  // The vertex position in world space.
	vec3 normal_ws;		  // The vertex normal in world space.
	vec2 tex_coord;		  // The vertex texture coordinates.
} out_data[];

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Passes the position, tangent, and bitangent from in_data to out_data.
	out_data[gl_InvocationID].position_ws = in_data[gl_InvocationID].position_ws;
	out_data[gl_InvocationID].normal_ws = in_data[gl_InvocationID].normal_ws;
	out_data[gl_InvocationID].tex_coord = in_data[gl_InvocationID].tex_coord;
	// Computes the tessellation factor.
	if (gl_InvocationID == 0)
	{		
		float tessellation_factor = 2048;
		// Sets all tessellation factors to 'tessellation_factor'.
		gl_TessLevelInner[0] = tessellation_factor;
		gl_TessLevelOuter[0] = tessellation_factor;
		gl_TessLevelOuter[1] = tessellation_factor;
		gl_TessLevelOuter[2] = tessellation_factor;
	}
}