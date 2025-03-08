#version 410 core
layout (location = 0) in vec3 positions;
layout (location = 1) in vec3 colors;
layout (location = 2) in vec2 texture_coords;

out vec3 frag_colors;
out vec2 frag_texture_coords;

void main() {
	gl_Position = vec4(positions, 1.0);
	frag_colors = colors;
	frag_texture_coords = texture_coords;
}
