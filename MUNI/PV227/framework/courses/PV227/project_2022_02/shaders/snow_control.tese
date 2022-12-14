#version 450 core

// The input primitive specification.
//layout(quads, fractional_odd_spacing, ccw) in;
layout(triangles, equal_spacing, ccw) in;

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
} out_data;

// The height texture and a scaling factor applied to it.
layout (binding = 1) uniform sampler2D height_tex;
layout(binding = 2) uniform sampler2D shadow_tex;
layout(binding = 4) uniform sampler2D snow_height_tex;
uniform mat4 shadow_matrix;
// ----------------------------------------------------------------------------
// Custom Methods
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	vec3 p = gl_TessCoord.x * in_data[0].position_ws.xyz + gl_TessCoord.y * in_data[1].position_ws.xyz + gl_TessCoord.z * in_data[2].position_ws.xyz;
	vec4 shadow_tex_coord = shadow_matrix * vec4(p, 1.0);
	float accumulated_snow = texture(height_tex, shadow_tex_coord.xy).x;
	float snow_height = texture(snow_height_tex, shadow_tex_coord.xy).r;
	float t = texture(shadow_tex, shadow_tex_coord.xy).r >= (shadow_tex_coord.z) - 0.05f ? 1.0 : 0.0;

	out_data.normal_ws = gl_TessCoord.x * in_data[0].normal_ws +  gl_TessCoord.y * in_data[1].normal_ws +  gl_TessCoord.z * in_data[2].normal_ws;
	out_data.position_ws = p;
	out_data.position_ws.y += t * abs(texture(shadow_tex, shadow_tex_coord.xy).r - (shadow_tex_coord.z));
	out_data.position_ws.y += accumulated_snow * snow_height * t;
	
	out_data.tex_coord = shadow_tex_coord.xy;
	//out_data.tex_coord = gl_TessCoord.x * in_data[0].tex_coord + gl_TessCoord.y * in_data[1].tex_coord + gl_TessCoord.z * in_data[2].tex_coord;
	// Computes gl_Position.
	gl_Position = projection * view * vec4(out_data.position_ws, 1.0);
}