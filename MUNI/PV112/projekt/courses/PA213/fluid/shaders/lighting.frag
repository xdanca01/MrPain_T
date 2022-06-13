#version 430 core

in VertexData { vec2 tex_coord; } in_data;      // UV coordinates of the rendered texel.
layout(location = 0) out vec4 final_color;      // Final colour of the rendered pixel. You must write your result to this variable!!
uniform sampler2D depth_texture;                // IGNORE!
uniform sampler2D normals_texture;              // The texture holding normal vectors as compuded by "normals.frag" shader.
uniform vec4 ambient_color;                     // The ambient colour.
uniform vec3 light_direction;                   // The unit vector towards the only light in the scene.
uniform vec4 light_color;                       // The colour of the light.
uniform vec4 diffuse_coef;                      // The diffuse coefficient "k_d" of the fluid's surface.
uniform vec4 specular_coef;                     // The specular coefficient "k_s" of the fluid's surface.
uniform float specular_exponent;                // The specular exponent of the fluid's surface.
uniform mat4 projection_matrix_inverted;        // IGNORE!

// Given UV coordinates of a given texel and the corresponding
// depth value from the 'depth_texture' (i.e., in range <0,1>), the function
// computes the corresponding point in camera (eye/view) space.
vec3 to_eye_space(vec2 tex_coord01, float z01)
{
    vec4 point_ndc = vec4(2 * tex_coord01 - 1, 2 * z01 - 1, 1);
    vec4 point_eye_w = projection_matrix_inverted * point_ndc;
    vec3 point_eye = point_eye_w.xyz / point_eye_w.w;
    return point_eye;
}

// Given UV coordinates of a given texel, the function computes the corresponding
// point in camera (eye/view) space.
vec3 to_eye_space(vec2 tex_coord)
{
    return to_eye_space(tex_coord, texture(depth_texture, tex_coord).r);
}

void main()
{
    // We discard texels which do not correspond to any particle.
    float self_orig_z = texture(depth_texture, in_data.tex_coord).r;
    if (self_orig_z == 1.0)
    {
        discard;
        return;
    }
    final_color = vec4(0,0.75f,0,1);
    // --- TASK BEGIN ------------------------------------------------------
    // Read the unit normal vector from the 'normals_texture' texture (do not
    // forget to transform the coordinates from <0,1>) and apply Blinn-Phong
    // lighting model at the rendered texel. The lighting should be computed
    // in the camera space. The final colour should be written to 'final_color'
    // variable.
    // HINT: Look at the slide #31 of the "Fluids" lecture for more info.
    vec3 P = to_eye_space(in_data.tex_coord);
    vec4 n = texture(normals_texture, in_data.tex_coord), L, V, H, S, D;
    // <-1,1>
    n = n*2 - 1;
    V = vec4(-P, 0);
    L = vec4(light_direction, 0);
    H = (L+V)/length(L+V);
    S = (light_color * specular_coef)*pow(dot(n, H), specular_exponent);
    D = (light_color*diffuse_coef)*max(0, (dot(n, L)));
    final_color = ambient_color + D + S;

    
    // --- TASK END ------------------------------------------------------

    gl_FragDepth = self_orig_z;
}
