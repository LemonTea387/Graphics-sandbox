#version 410 core
layout (location = 0) in vec3 a;

void main() {
	gl_Position = vec4(a.xyz, 1.0);
}
