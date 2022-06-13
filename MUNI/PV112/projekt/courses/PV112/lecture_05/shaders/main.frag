#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// Task 5.2: Add a uniform variable for eye(camera) position.
// Camera position
layout(location = 3) uniform vec3 camera;
// Task 5.2: Add a uniform variable for light's position and ambient+diffuse+specular terms.
layout(location = 4) uniform vec4 light_position;
layout(location = 5) uniform vec3 light_ambient;
layout(location = 6) uniform vec3 light_diffuse;
layout(location = 7) uniform vec3 light_specular;
layout(location = 8) uniform vec3 material_ambient;
layout(location = 9) uniform vec3  material_diffuse;
layout(location = 10) uniform vec3 material_specular;
layout(location = 11) uniform float shine;
// Task 5.2: Add a uniform variable for material's ambient+diffuse+specular+shininess terms.

// The intpolated position from the vertex shader.
layout(location = 0) in vec3 fs_position;
// The intpolated normal from the vertex shader.
layout(location = 1) in vec3 fs_normal;
// The intpolated texture coordinates from the vertex shader.
layout(location = 2) in vec2 fs_texture_coordinate;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// Task 5.2: Compute (N)ormal, (L)ight, (E)ye, (H)alf vectors.
	// Task 5.2: Compute dot products between N and L, N and H.
	// Task 5.2: Compute material values multiplied by light's values.
	// Task 5.2: Compute final color using Blinn�Phong shading model.
	// Task 5.2: Use attenuation based on the light distance for point lights.
	// Task 5.2: Differentiate between directional and point light based on its .w value in position.

	vec3 ambient = light_ambient * material_ambient;
	vec3 norm = normalize(fs_normal);
	vec3 lightDirection = normalize(light_position.xyz - fs_position * light_position.w);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = light_diffuse * diff * material_diffuse;
	vec3 camera_to_pos = normalize(camera - fs_position);
	vec3 reflect_light = reflect(-lightDirection, norm);
	float spec = pow(max(dot(camera_to_pos, reflect_light), 0.0), shine);
	vec3 specular = light_specular * spec * material_specular;
	vec3 color = (ambient + diffuse + specular);
	final_color = vec4(color, 1.0);
}
