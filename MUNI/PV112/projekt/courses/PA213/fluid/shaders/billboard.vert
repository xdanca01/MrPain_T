#version 430 core

layout(location = 0) in vec3 position;
layout(location = 0) out vec4 colour;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform vec3 light_direction;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);

    vec3 normal = vec3(0,0,1);
    float z = (view * model * vec4(position, 1.0)).z;
    float t = min(max(1 - z*z / 100, 0), 1);
    colour = vec4(vec3(0.5, 0, 0) + vec3(0.25, 0.25, 0.25) * t, 1);
//colour = vec4(vec3(0.25, (((10 * gl_VertexID + 70)) % 100) / 100.0, (((20 * gl_VertexID + 30)) % 200) / 200.0), 1);
}
