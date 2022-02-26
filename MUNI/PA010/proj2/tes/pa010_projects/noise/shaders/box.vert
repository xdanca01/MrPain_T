#version 330

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 in_uv;
out vec2 uv;

void main()
{
    uv = in_uv;
    gl_Position = projection * view * model * vec4(position, 1.0);
}
