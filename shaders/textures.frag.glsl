#version 410
out vec4 frag_color;

in vec3 frag_colors;
in vec2 frag_texture_coords;

uniform sampler2D uniform_texture;

void main() {
	frag_color = vec4(frag_colors, 1.0) * texture(uniform_texture, frag_texture_coords);
}
