#version 330

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colour;
out vec3 axe_colour;

void main()
{
    axe_colour = position;
    gl_Position = projection * view * model * vec4(position, 1.0);
}