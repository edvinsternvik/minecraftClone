#version 330 core
layout (location = 0) in uint data;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform ivec2 u_chunkPosition;

out vec2 texCoords;
out float light;

void main() {
	float x = float((data & 0xF8000000u) >> 27);
	float y = float((data & 0x7F80000u) >> 19);
	float z = float((data & 0x7C000u) >> 14);
	uint facingDirection = (data & 0x3800u) >> 11;
	uint textureId = (data & 0x7FCu) >> 2;
	uint textureOffsetIndex = data & 0x3u;

	vec3 position = vec3(x + u_chunkPosition.x * 16, y, z + u_chunkPosition.y * 16);
	vec4 fragPos = vec4(position, 1.0);

	float textureX = float(textureId % 32u);
	float textureY = 0.0;
	float textureOffsetX = textureOffsetIndex == 0u || textureOffsetIndex == 3u ? 0.0 : 1.0;
	float textureOffsetY = textureOffsetIndex < 2u ? 0.0 : 1.0;
	texCoords = vec2(textureX + textureOffsetX, textureY + textureOffsetY);

	light = 0.8;
	if(facingDirection == 4u) light = 1.0;
	if(facingDirection == 5u) light = 0.5;

	gl_Position = u_projection * u_view * fragPos;
}