#version 430 core

in VertexData { vec2 tex_coord; } in_data;  // UV coordinates of the rendered texel.
layout(location = 0) out vec4 final_color;  // IGNORE!
uniform sampler2D depth_texture;            // The depth buffer of rendered particles by "depth.frag" shader.
uniform mat4 projection_matrix;             // IGNORE!
uniform mat4 projection_matrix_inverted;    // IGNORE!
uniform float particle_radius;              // Radius of any fluid particle in meters.
uniform int smoothing_radius;               // The maximal smoothing radius (max. number of texels), i.e., maximum for du and dv. See slides #26-27.
uniform float smooting_gauss_sigma;         // Sigma common to both Gauss kernels. See slide #25.
uniform float smooting_depth_falloff;       // The falloff coeficient of the range kernel. See slide #25.

// Given uv coordinates of a given texel and the corresponding
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
// point in the camera (eye/view) space.
vec3 to_eye_space(vec2 tex_coord)
{
    return to_eye_space(tex_coord, texture(depth_texture, tex_coord).r);
}

// Given a point in camera (eye/view) space, the function project that
// point and return UV coordinates in screen space (each coordinate
// in range <0,1>).
vec4 to_tex_space(vec3 eye_space_pos)
{
    vec4 clip_pos = projection_matrix * vec4(eye_space_pos, 1);
    vec4 ndc_pos = clip_pos / clip_pos.w;
    vec4 tex_pos = 0.5 * ndc_pos + 0.5;
    return tex_pos;
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

    gl_FragDepth = self_orig_z; // This is not the right solution.

    // --- TASK END ------------------------------------------------------

    float show_z = pow(gl_FragDepth, 10);
    final_color = vec4(show_z, show_z, show_z, 1);
}
