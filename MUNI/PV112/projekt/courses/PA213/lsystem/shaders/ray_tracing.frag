#version 450 core

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------
in VertexData
{
	vec2 tex_coord;
} in_data;

// The UBO with camera data.	
layout (std140, binding = 0) uniform CameraBuffer
{
	mat4 projection;	  // The projection matrix.
	mat4 projection_inv;  // The inverse of the projection matrix.
	mat4 view;			  // The view matrix
	mat4 view_inv;		  // The inverse of the view matrix.
	mat3 view_it;		  // The inverse of the transpose of the top-left part 3x3 of the view matrix
	vec3 eye_position;	  // The position of the eye in world space.
};

// The structure holding the information about a single Phong light.
struct PhongLight
{
	vec4 position;                   // The position of the light. Note that position.w should be one for point lights and spot lights, and zero for directional lights.
	vec3 ambient;                    // The ambient part of the color of the light.
	vec3 diffuse;                    // The diffuse part of the color of the light.
	vec3 specular;                   // The specular part of the color of the light. 
	vec3 spot_direction;             // The direction of the spot light, irrelevant for point lights and directional lights.
	float spot_exponent;             // The spot exponent of the spot light, irrelevant for point lights and directional lights.
	float spot_cos_cutoff;           // The cosine of the spot light's cutoff angle, -1 point lights, irrelevant for directional lights.
	float atten_constant;            // The constant attenuation of spot lights and point lights, irrelevant for directional lights. For no attenuation, set this to 1.
	float atten_linear;              // The linear attenuation of spot lights and point lights, irrelevant for directional lights.  For no attenuation, set this to 0.
	float atten_quadratic;           // The quadratic attenuation of spot lights and point lights, irrelevant for directional lights. For no attenuation, set this to 0.
};

// The UBO with light data.
layout (std140, binding = 2) uniform PhongLightsBuffer
{
	vec3 global_ambient_color;		// The global ambient color.
	int lights_count;				// The number of lights in the buffer.
	PhongLight lights[8];			// The array with actual lights.
};

// The definition of branch as rounded cone.
struct Branch
{
    vec3 p1;  // The position of the bottom point.
    float r1; // The bottom radius.
    vec3 p2;  // The position of the top point.
    float r2; // The upper radius.
};

 // The buffer with individual branches defining the tree.
layout (std140, binding = 4) uniform TreeBuffer
{
   Branch branches[100];
};
// The number of branches.
uniform int num_branches;

// The definition of leaf.
struct Leaf
{
    vec4 position;   // The position of the leaf (.w is used only for padding).
    vec4 direction;  // The direction of the leaf (.w is used only for padding).
    vec4 up;		 // The up vector for the leaf (.w is used only for padding).
    vec4 size;		 // The size of the leaf (.zw are used only for padding).
};

 // The buffer with individual branches defining the tree.
layout (std140, binding = 5) uniform LevesBuffer
{
   Leaf leaves[100];
};
// The number of leaves.
uniform int num_leaves;

// The windows size.
uniform vec2 resolution;

// The skybox texture.
layout (binding = 0) uniform samplerCube skybox_tex; 
// The wood texture.
layout (binding = 1) uniform sampler2D wood_tex; 
// The leaf texture.
layout (binding = 2) uniform sampler2D laef_tex; 

/** The flag determining if a tone mapping should be used. */
uniform bool use_tone_mapping;
/** The exposure value. */
uniform float exposure;
/** The gamma value. */
uniform float gamma;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// The final output color.
layout (location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Ray Tracing Structures
// ----------------------------------------------------------------------------
// The definition of a ray.
struct Ray {
    vec3 origin;     // The ray origin.
    vec3 direction;  // The ray direction.
};
// The definition of an intersection.
struct Hit {
    vec3 intersection;        // The intersection point.
	float t;				  // The distance between the ray origin and the intersection points along the ray. 
    vec3 normal;              // The surface normal at the interesection point.
	vec3 material;			  // The material of the object at the intersection point.
};
const Hit miss = Hit(vec3(0.0), 1e20, vec3(0.0), vec3(0.f));

// ----------------------------------------------------------------------------
// Local Methods
// ----------------------------------------------------------------------------

// Computes the branch color based on UV coordinates.
// intersection - the calculated intersection between the branch and the ray
// branch - the branch itself
vec3 getBranchMaterial(vec3 intersection, Branch branch){
	// ----------------- TODO: Write your implementation here! ----------------- //
	// Return correct color for the intersection using texture from 'wood_tex'.
	// Hint: Note that you may need to scale the UVs to get a better looking 
	//       result - (0.2, 0.5) works good.
	// ------------------------------------------------------------------------- //
	vec3 q = intersection - branch.p1;
	vec3 w = branch.p2 - branch.p1;
	vec3 t = cross(w, vec3(0,0,1));
	vec3 s = cross(w, t);
	float v = dot(w, q);
	float u = atan(dot(q, t), dot(q, s));
	return texture(wood_tex, vec2(u*0.2, v*0.5)).rgb;
}

// Computes an intersection between a ray and a branch (defined as rounded cone).
// ray - the ray definition (contains ray.origin and ray.direction)
// cone - the rounded cone 
Hit RayBranchIntersection(Ray ray, Branch branch) {
	// The basic vectors.
	vec3  ab = branch.p2 - branch.p1;
	vec3  ao = ray.origin - branch.p1;
	vec3  bo = ray.origin - branch.p2;
    float rr = branch.r1 - branch.r2;
    
	// Projections and square magnitudes for the vectors.
	float m0 = dot(ab,ab);
    float m1 = dot(ab,ao); 
    float m2 = dot(ab,ray.direction);
    float m3 = dot(ray.direction,ao);
    float m5 = dot(ao,ao); 
	float m6 = dot(bo,ray.direction);
    float m7 = dot(bo,bo);

	// Solving the intersection.
	float d2 = m0 - rr * rr;
	float k0 = d2*m5 - m1 * m1 + m1 * rr * branch.r1 * 2.0 - m0 * branch.r1 * branch.r1;
    float k1 = d2*m3 - m1 * m2 + m2 * rr * branch.r1;
	float k2 = d2 - m2 * m2;
	float h = k1*k1 - k0 * k2;
	if(h < 0.0) return miss; // no intersection
    
	// We have intrsection, solving where exactly.
	float t = (-sqrt(h) - k1) / k2;
    float y = m1 - branch.r1 * rr + t * m2;

	// Cone.
    if(y > 0.0 && y < d2) 
    {
		vec3 intersection = ray.origin + t * ray.direction;
		vec3 normal = normalize(d2 * (ao + t * ray.direction) - ab * y);
		if(t < 0) return miss;
		return Hit(intersection, t, normal, getBranchMaterial(intersection, branch));
    }

	// One of the spheres.
    float h1 = m3 * m3 - m5 + branch.r1 * branch.r1;
    float h2 = m6 * m6 - m7 + branch.r2 * branch.r2;
    if(max(h1,h2) < 0.0) miss;
    Hit hit = miss;

	// Sphere at p1.
	if(h1 > 0.0)
    {        
		t = -m3 - sqrt(h1);
		vec3 intersection = ray.origin + t * ray.direction;
		vec3 normal = (ao+t*ray.direction)/branch.r1;
		hit = Hit(intersection, t, normal, getBranchMaterial(intersection, branch));
	}
	// Sphere at p2.
	if(h2 > 0.0)
    {
    	t = -m6 - sqrt(h2);
        if(t < hit.t){
			vec3 intersection = ray.origin + t * ray.direction;
			vec3 normal = (bo+t*ray.direction)/branch.r2;
			hit = Hit(intersection, t, normal, getBranchMaterial(intersection, branch));
		}
    }

	// Makes sure we consider intersection only in the positive direction.
	if(hit.t < 0) return miss;
    return hit;
}

// Computes an intersection between a ray and a plane defined by its normal and one point inside the plane.
// ray - the ray definition (contains ray.origin and ray.direction)
// normal - the plane normal
// point - a point laying in the plane
Hit RayPlaneIntersection(Ray ray, vec3 normal, vec3 point) {
	float nd = dot(normal, ray.direction);
	vec3 sp = point - ray.origin;
	float t = dot(sp, normal) / nd;
    if (t < 0.0) return miss;

	vec3 intersection = ray.origin + t * ray.direction;
	
	// circle
	if(length(intersection) > 8) 
		return miss;

	return Hit(intersection, t, normal, vec3(0.5));
}

// Computes an intersection between a ray and a leaf (defined as parallelogram).
// ray - the ray definition (contains ray.origin and ray.direction)
// leaf - the leaf
Hit RayLeafIntersection(Ray ray, Leaf leaf){

	// Left vector;
	vec3 left = normalize(cross(leaf.up.xyz, leaf.direction.xyz));
	vec3 v0 = leaf.position.xyz - left * leaf.size.x / 2.0f;

	// Parallelogram vectors.
    vec3 a = -leaf.direction.xyz * leaf.size.y;
    vec3 b = left * leaf.size.x;

	// Parallelogram basis (not normalized !!!).
    vec3 p = v0 - ray.origin;
    vec3 q = cross(ray.direction, p);
	vec3 n = cross(a,b);

	// The position of the intersection relative to parallelogram vectors
    float i = 1.0 / dot(ray.direction, n);
    float u = dot(q, a)*i;
    float v = dot(q, b)*i;
    float t = dot(n, p)*i;

	// Checking if the points is inside the parallelogram.
    if(u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) return miss;
    
	// Intersection.
	vec3 intersection = ray.origin + t * ray.direction;
	// Makes sure we consider intersection only in the positive direction.
	if(t < 0.0) return miss;

	// ----------------- TODO: Write your implementation here! ----------------- //
	// Apply the leaf texture from 'laef_tex'.
	// Hint: You may need to update the normal to achive a proper diffuse shading.
	// ------------------------------------------------------------------------- //
	vec4 leaf_t = texture(laef_tex, vec2(u, v));
	//Alpha == 0, its not hit, because there is nothing on this position
	if(leaf_t.a <= 0.1)
		return miss;
	//we need to switch direction of normal
	if(dot(ray.direction, n) > 0)
		n = -n;
	return Hit(intersection, t, normalize(n), leaf_t.rgb);
}

// Evaluates the intersections of the ray with the scene objects and returns the closes hit.
Hit Evaluate(Ray ray){
	// Sets the closes hit either to miss or to an intersection with the plane representing the ground.
	Hit closest_hit = RayPlaneIntersection(ray, vec3(0, 1, 0), vec3(0));

	for(int i = 0; i < num_branches; i++){
		Hit intersection = RayBranchIntersection(ray, branches[i]);
		if(intersection.t < closest_hit.t){
			closest_hit = intersection;
		}
	}

	for(int i = 0; i < num_leaves; i++){
		Hit intersection = RayLeafIntersection(ray, leaves[i]);
		if(intersection.t < closest_hit.t){
			closest_hit = intersection;
		}
	}

    return closest_hit;
}

// Traces the ray trough the scene and accumulates the color.
vec3 Trace(Ray ray) {
    // The accumulated color and attenuation used when tracing the rays throug the scene.
	vec3 color = vec3(0.0);
    vec3 attenuation = vec3(1.0);

	// The direction towards the light.
	vec3 L = normalize(lights[0].position.xyz);

	// Due to floating-point precision errors, when a ray intersects geometry at a surface, the point of intersection could possibly be just below the surface.
	// The subsequent reflection and shadow rays would then bounce off the *inside* wall of the surface. This is known as self-intersection.
	// We, therefore, use a small epsilon to offset the subsequent rays.
	const float epsilon = 0.01;

	// ----------------- TODO: Write your implementation here! ----------------- //
	// Add difuse lighting, shadows and skybox.
	// Hints: The skybox texture is stored in 'skybox_tex'.
	//        You can keep a bit of color in areas with shadows (e.g., 0.2*hit.material).
	// ------------------------------------------------------------------------- //
	Hit hit = Evaluate(ray);
	if (hit != miss) {
		Ray shadow_ray = Ray(hit.intersection, L);
		Hit second_hit = Evaluate(shadow_ray);
		//Revaluate because other side blocked
		if(distance(hit.intersection, second_hit.intersection) <= epsilon){
			shadow_ray.origin = second_hit.intersection + L*epsilon;
			second_hit = Evaluate(shadow_ray);
		}
		//Light
		if(second_hit == miss){
			
			vec3 A = 0.2 * hit.material;
			vec3 D = 0.8 * lights[0].diffuse * hit.material * max(dot(hit.normal, L), 0);
			color = A + D;
		}
		//Shadow
		else
			color = 0.2 * hit.material;
	} 
	//SKYBOX
	else{
		color = texture(skybox_tex, ray.direction).rgb;
	}
    return color;
}

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	// The aspect ratio.
	// It tells us how many times the window is wider (or narrower) w.r.t. its height.
	float aspect_ratio = resolution.x/resolution.y;

	// We use the texture coordinates and the aspect ratio to map the coordinates to the screen space.
	vec2 uv = (2.0*in_data.tex_coord - 1.0) * vec2(aspect_ratio, 1.0);

	// Computes the ray origin and ray direction using the view matrix.
	vec3 P = vec3(view_inv * vec4(uv, -1.0, 1.0));
	vec3 direction = normalize(P - eye_position);
	Ray ray = Ray(eye_position, direction);

	// We pass the ray to the trace function.
	vec3 color = Trace(ray);

	// Tone mapping.
	if(use_tone_mapping){
		vec3 mapped = vec3(1.0f) - exp2(-color * exposure);
		color = pow(mapped, vec3(1.0 / gamma));
	}

	final_color = vec4(color, 1.0);
}