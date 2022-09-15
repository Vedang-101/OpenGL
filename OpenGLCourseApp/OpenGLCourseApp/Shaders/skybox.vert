#version 330

layout (location = 0) in vec3 pos;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
	TexCoord = pos;
	gl_Position = projection * view * vec4(pos, 1.0);
}