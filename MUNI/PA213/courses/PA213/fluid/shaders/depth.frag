#version 430 core

layout(location = 0) in vec4 position;      // Position of the rendered texel (at "tex_coords") in camera (eye/view) space.
layout(location = 2) in vec2 tex_coords;    // Texture coordinates of the rendered texel (NOTE: both are in <0,1>!)
layout(location = 0) out vec4 final_color;  // IGNORE!

uniform mat4 projection_matrix; // You need this to transform a Sphere point from camera space to NDC.
uniform float particle_radius;  // In meters.

void main()
{
    // --- TASK BEGIN ------------------------------------------------------
    // Given the "tex_coords" in the rendered billboard, you are supposed
    // to determine whether the texel lies on the spere. If it does, then
    // compute its Z coordinate and write it to the "gl_FragDepth" variable.
    // Otherwise, use the keyword "discard" to discard the rendered texel.
    // HINT: Look at slide #22 of the "Fluids" lecture for info.

    gl_FragDepth = 0.5 * ((projection_matrix * position).z / (projection_matrix * position).w) + 0.5; // This is not the right solution.

    // --- TASK END ------------------------------------------------------

    float show_z = max(min(pow(gl_FragDepth, 10),1),0);
    final_color = vec4(show_z, show_z, show_z, 1);
}
