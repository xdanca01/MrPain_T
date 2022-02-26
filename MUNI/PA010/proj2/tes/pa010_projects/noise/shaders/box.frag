#version 330

in vec2 uv;
layout(location = 0) out vec4 final_color;

uniform sampler2D noise_texture;

void main()
{
    final_color = texture(noise_texture, uv);
}
