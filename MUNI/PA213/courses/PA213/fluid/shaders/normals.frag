#version 430 core

in VertexData { vec2 tex_coord; } in_data;      // UV coordinates of the rendered texel.
layout(location = 0) out vec4 final_color;      // IGNORE!
uniform sampler2D depth_texture;                // The depth buffer of rendered particles smoothed by "smooth.frag" shader.
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

// Given the point "origin" in camera (eye/view) space corresponding to the
// rendered texel and a shift in U or V direction in the screen space,
// the function computes a derivative of the surface of the fluid
// in the direction corresponding to the U/V shift. 
vec3 position_delta(vec3 origin, vec2 delta)
{
    // --- TASK BEGIN ------------------------------------------------------
    // Implement the functionality described in the comment above the function.
    // HINT: Look at slides #29-#30 of the "Fluids" lecture for more info.

    return vec3(0,0,0); // This is not the right solution.

    // --- TASK END ------------------------------------------------------
}

void main()
{
    // We need to know the size of the texture and size of a texel.
    ivec2 texture_size = textureSize(depth_texture, 0);
    vec2 pixel_delta = vec2(1.0/texture_size.x, 1.0/texture_size.y);

    // Declaration of the output variable of our task in this shader.
    // It means that once we compute the unit normal vector for the
    // rendered texel, we are supposed to write the vector into this
    // variable.
    vec3 normal_rgb;

    // --- TASK BEGIN ------------------------------------------------------
    // The goal is to compute the unit normal vector at the rendered texel
    // and write the result into the variable 'normal_rgb'. Do not forget
    // to convert the coordinates into <0,1> range.
    // HINT: Look at slides #28-#30 of the "Fluids" lecture for more info.

    normal_rgb = texture(depth_texture, in_data.tex_coord).r < 1.0f ? vec3(0,1,0) : vec3(0.5f,0.5f,1.0f); // This is not right solution.

    // --- TASK END ------------------------------------------------------

    final_color = vec4(normal_rgb, 1);
}
