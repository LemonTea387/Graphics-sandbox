#version 410
out vec4 frag_color;

in vec3 frag_colors;
in vec2 frag_texture_coords;

uniform sampler2D uniform_texture1;
uniform sampler2D uniform_texture2;

void main() {
	frag_color = vec4(frag_colors, 1.0) * mix(texture(uniform_texture1, frag_texture_coords), texture(uniform_texture2, frag_texture_coords), 0.2);
}
