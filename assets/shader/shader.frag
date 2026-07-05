#version 330 core
out vec4 out_color;

uniform sampler2D texture1;
in vec2 pass_texcoords;

void main()
{
   out_color = texture(texture1, pass_texcoords);
}