#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec2 pass_texcoords;

void main()
{
   gl_Position = projection_mat * view_mat * model_mat * vec4(position, 1.0);
   pass_texcoords = texcoords;
}