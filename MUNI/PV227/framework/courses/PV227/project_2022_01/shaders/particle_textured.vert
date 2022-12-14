#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
// The particle position.
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

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

uniform bool is_mirrored;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
out VertexData
{
	vec4 position_vs;  // The particle position in view space.
	vec4 color;        // The particle color.
	flat int id;       // The particle id.
} out_data;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	mat4 new_view = view;
	if(is_mirrored == true){
		new_view = view * mat4(1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	}
	vec4 pos = new_view * position;
	if((is_mirrored == false && position.y < 0.0)){
		pos.w = 0.0;
	}	
	out_data.position_vs = pos;
	out_data.color = color;
	out_data.id = gl_VertexID;
}