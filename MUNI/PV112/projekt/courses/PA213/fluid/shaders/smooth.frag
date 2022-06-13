#version 430 core

in VertexData { vec2 tex_coord; } in_data;  // UV coordinates of the rendered texel.
layout(location = 0) out vec4 final_color;  // IGNORE!
uniform sampler2D depth_texture;            // The depth buffer of rendered particles by "depth.frag" shader.
uniform mat4 projection_matrix;             // IGNORE!
uniform mat4 projection_matrix_inverted;    // IGNORE!
uniform float particle_radius;              // Radius of any fluid particle in meters.
uniform int smoothing_radius;               // r smooth The maximal smoothing radius (max. number of texels), i.e., maximum for du and dv. See slides #26-27.
uniform float smooting_gauss_sigma;         // Sigma common to both Gauss kernels. See slide #25.
uniform float smooting_depth_falloff;       // The falloff coeficient of the range kernel. See slide #25.

// Given uv coordinates of a given texel and the corresponding
// depth value from the 'depth_texture' (i.e., in range <0,1>), the function
// computes the corresponding point in camera (eye/view) space.
//f inv
vec3 to_eye_space(vec2 tex_coord01, float z01)
{
    vec4 point_ndc = vec4(2 * tex_coord01 - 1, 2 * z01 - 1, 1);
    vec4 point_eye_w = projection_matrix_inverted * point_ndc;
    vec3 point_eye = point_eye_w.xyz / point_eye_w.w;
    return point_eye;
}

// Given UV coordinates of a given texel, the function computes the corresponding
// point in the camera (eye/view) space.
vec3 to_eye_space(vec2 tex_coord)
{
    return to_eye_space(tex_coord, texture(depth_texture, tex_coord).r);
}

// Given a point in camera (eye/view) space, the function project that
// point and return UV coordinates in screen space (each coordinate
// in range <0,1>).
//f
vec4 to_tex_space(vec3 eye_space_pos)
{
    vec4 clip_pos = projection_matrix * vec4(eye_space_pos, 1);
    vec4 ndc_pos = clip_pos / clip_pos.w;
    vec4 tex_pos = 0.5 * ndc_pos + 0.5;
    return tex_pos;
}

float gauss_kernel(float x)
{
    //     e^ (-x^2        /2*sigma^2)                 /  2*pi*sigma^2
    return exp(-(pow(x, 2) / (2*pow(smooting_gauss_sigma, 2)))) / (2*3.14*pow(smooting_gauss_sigma, 2));
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

    // We need to know the size of the texture and size of a texel.
    ivec2 texture_size = textureSize(depth_texture, 0);
    //epsilon
    vec2 pixel_delta = vec2(1.0/texture_size.x, 1.0/texture_size.y);

    // --- TASK BEGIN ------------------------------------------------------
    // The task is to first determine range [-du,...,du]x[-dv,...,dv] rectangle
    // in the screen space of texels in which we consider the computation (smoothing).
    // HINT: Look at slides #26 and #27 of the "Fluids" lecture for more info.
    //       You can also use to_eye_space / to_tex_space methods defined above.
    //
    // The core of the computation in the rectangle is the application of
    // a bilateral filter. The result is a new (smoothed) Z coordinate for the
    // rendered texel. This Z coordinate must be written to the 'gl_FragDepth'
    // variable.
    // HINT: Look at slides #23-#27 of the "Fluids" lecture for more info.
    // 
    // (Optional) Some of the points in the rectangle should be excluded
    // when soothing - see bottom of slide #27.
    float W = 0.0f, z_smooth = 0.0f, z1, z2, length_pq, delta_u, delta_v;
    z2 = texture(depth_texture, vec2(in_data.tex_coord.x, in_data.tex_coord.y)).r;
    vec3 P = to_eye_space(in_data.tex_coord), r = vec3(particle_radius, particle_radius, 0), U, L;
    vec4 U_l, L_l;
    U = P + r;
    L = P - r;
    U_l = to_tex_space(U);
    L_l = to_tex_space(L);
    delta_u = int(min((U_l.x - L_l.x)/(2*pixel_delta.x), smoothing_radius));
    delta_v = int(min((U_l.y - L_l.y)/(2*pixel_delta.y), smoothing_radius));
    for(float p = -delta_u; p <= delta_u;++p)
    {
        for(float q = -delta_v; q <= delta_v;++q)
        {
            z1 = texture(depth_texture, in_data.tex_coord+vec2(p*pixel_delta.x, q*pixel_delta.y)).r; 
            if(z1 == 1) continue;
            length_pq = length(vec2(p, q));
            W += gauss_kernel(abs(z1-z2)*smooting_depth_falloff) * gauss_kernel(length_pq/smoothing_radius);
            z_smooth += z1 * gauss_kernel(abs(z2 - z1)*smooting_depth_falloff) * gauss_kernel(length_pq/smoothing_radius);
        }
    }
    if(W != 0.0f)
    {
        gl_FragDepth = 1/W * z_smooth;
    }

    // --- TASK END ------------------------------------------------------

    float show_z = pow(gl_FragDepth, 10);
    final_color = vec4(show_z, show_z, show_z, 1);
}
