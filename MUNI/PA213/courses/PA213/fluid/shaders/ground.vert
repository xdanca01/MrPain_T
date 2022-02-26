#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 0) out vec4 colour;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_direction;

void main()
{
    vec3 N = (model * vec4(0.5 * normal,0)).xyz;
    colour = vec4(vec3(0.5, 0.5, 0.5) + max(0, dot(N, light_direction)) * 0.5 * N, 1);
    gl_Position = projection * view * model * vec4(position, 1.0);
}
