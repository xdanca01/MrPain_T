#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_tex_coords;

layout(location = 0) out vec4 position;
layout(location = 2) out vec2 tex_coords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
    position = view * model * vec4(in_position, 1.0);
    tex_coords = in_tex_coords;
    gl_Position = projection * position;
}
