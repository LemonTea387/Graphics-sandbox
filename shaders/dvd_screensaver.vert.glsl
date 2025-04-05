#version 410 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoord;
uniform vec2 offset;

out vec2 passedTexCoord;

void main() {
	gl_Position = vec4(Pos.xy + offset, Pos.z, 1.0);
	passedTexCoord = TexCoord;
}
