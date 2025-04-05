#version 410 core
out vec4 FragColor;
in vec2 passedTexCoord;
uniform vec3 color;
uniform sampler2D uniform_texture;

void main(){
	FragColor = vec4(color, 1.0) * texture(uniform_texture, passedTexCoord);
}


