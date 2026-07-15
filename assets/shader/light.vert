#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

out vec3 frag_pos;
out vec3 pass_normal;

void main()
{
	gl_Position = projection_mat * view_mat * model_mat * vec4(position, 1.0);
	frag_pos = vec3(model_mat * vec4(position, 1.0));
	pass_normal = mat3(transpose(inverse(model_mat))) * normal;
}