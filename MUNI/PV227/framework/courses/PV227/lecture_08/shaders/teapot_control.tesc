#version 450 core

// The output primitive specification.
layout(vertices = 16) out;

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec3 position_ws;	  // The vertex position in world space.
	vec3 tangent_ws;	  // The vertex tangent in world space.
	vec3 bitangent_ws;	  // The vertex bitangent in world space.
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

// The uniform parameters.
uniform vec2 window_size;				// The size of the window in pixels.
uniform bool auto_tessellation;			// The flag determining if auto tesselation should be used.
uniform float triangle_size;			// The approximative size of one triangle in pixels when auto_tessellation is true.
uniform float tessellation_factor;		// The tessellation factor when auto_tessellation is false.

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
out VertexData
{
	vec3 position_ws;	  // The vertex position in world space.
	vec3 tangent_ws;	  // The vertex tangent in world space.
	vec3 bitangent_ws;	  // The vertex bitangent in world space.
} out_data[];

// The positions in screen space (i.e. after dividing by w and after multiplying by window size). 
// Only to pass the data between invocations, not used in the evaluation shader itself.
out vec2 position_ss[];	

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Passes the position, tangent, and bitangent from in_data to out_data.
	out_data[gl_InvocationID].position_ws = in_data[gl_InvocationID].position_ws;
	out_data[gl_InvocationID].tangent_ws = in_data[gl_InvocationID].tangent_ws;
	out_data[gl_InvocationID].bitangent_ws = in_data[gl_InvocationID].bitangent_ws;

	// TASK 4: Compute the position of the control point in the screen space, and store it into shared 'position_ss' array.
	//   Hint: Store the result into position_ss[gl_InvocationID], don't forget to call barrier().

	vec4 tmp = projection * view * vec4(in_data[gl_InvocationID].position_ws, 1.0);
	position_ss[gl_InvocationID] = (tmp.xy / tmp.w) * window_size / 2;
	barrier();
	// Computes the tessellation factor.
	if (gl_InvocationID == 0)
	{
		if (auto_tessellation)
		{
			// TASK 4: Compute the tessellation factors based on the triangle size.
			//   Hint: You can use function 'length'.
			float Length = length(position_ss[0] - position_ss[4]) + length(position_ss[4] - position_ss[8]) + length(position_ss[8] - position_ss[12]);
			gl_TessLevelOuter[0] = max(1, Length / triangle_size);//;max(Length, 1);
			
			Length = length(position_ss[0] - position_ss[1]) + length(position_ss[1] - position_ss[2]) + length(position_ss[2] - position_ss[3]);
			gl_TessLevelOuter[1] = max(1, Length / triangle_size);//;max(Length, 1);

			Length = length(position_ss[15] - position_ss[11]) + length(position_ss[11] - position_ss[7]) + length(position_ss[7] - position_ss[3]);
			gl_TessLevelOuter[2] = max(1, Length / triangle_size);//;max(Length, 1);

			Length = length(position_ss[12] - position_ss[13]) + length(position_ss[13] - position_ss[14]) + length(position_ss[14] - position_ss[15]);
			gl_TessLevelOuter[3] = max(1, Length / triangle_size);//;max(Length, 1);

			Length = (gl_TessLevelOuter[1]+gl_TessLevelOuter[3])/2 + (gl_TessLevelOuter[0]+gl_TessLevelOuter[2])/2;
			gl_TessLevelInner[0] = max(1, Length / triangle_size);

			//Length = (gl_TessLevelOuter[0]+gl_TessLevelOuter[2])/2;
			gl_TessLevelInner[1] = max(1, Length / triangle_size);
						
		}
		else
		{
			// Sets all tessellation factors to 'tessellation_factor'.
			gl_TessLevelInner[0] = tessellation_factor;
			gl_TessLevelInner[1] = tessellation_factor;
			gl_TessLevelOuter[0] = tessellation_factor;
			gl_TessLevelOuter[1] = tessellation_factor;
			gl_TessLevelOuter[2] = tessellation_factor;
			gl_TessLevelOuter[3] = tessellation_factor;
		}
	}
}