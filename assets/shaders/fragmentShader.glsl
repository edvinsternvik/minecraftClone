#version 330 core
out vec4 FragColor;

in vec2 texCoords;
in float light;

uniform vec2 u_textureSize;
uniform sampler2D epicTexture;

void main() {
	vec2 textureCoords = u_textureSize * texCoords;
	vec4 texColor = texture(epicTexture, textureCoords);
	if(texColor.a < 0.1) discard;

	FragColor = texColor * light;
} 