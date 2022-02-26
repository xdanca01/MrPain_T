#version 330

layout(location = 0) out vec4 final_color;
in vec3 axe_colour;

void main()
{
    final_color = vec4(axe_colour, 1);
}
