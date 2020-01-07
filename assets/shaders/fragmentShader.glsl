#version 330 core
out vec4 FragColor;

// in vec3 lightingColor;
in vec2 texCoords;
in float light;

uniform vec2 u_textureSize;
uniform sampler2D epicTexture;

void main() {
	vec2 textureCoords = u_textureSize * texCoords;
	FragColor = texture(epicTexture, textureCoords) * light;
} 