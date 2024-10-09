#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
	vec4 world_position = vec4(position, 1) * M;
	vec4 final_pos = world_position * V * P;
	gl_Position = final_pos;
}