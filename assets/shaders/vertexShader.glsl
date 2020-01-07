#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 u_projection;
uniform mat4 u_view;
// uniform vec3 u_offset[4096];

// out vec3 lightingColor;
out vec2 texCoords;
out float light;

void main() {
	texCoords = aTexture;

	// vec4 fragPos = vec4(aPos + u_offset[gl_InstanceID], 1.0);
	vec4 fragPos = vec4(aPos, 1.0);
	// vec3 normal = normalize(mat3(transpose(inverse(u_model[gl_InstanceID]))) * aNormal);  

	// vec3 lightPos = vec3(0.0, 0.0, 0.0);
	// vec3 lightDir = normalize(lightPos - vec3(fragPos));

	// float lightAngle = max(dot(normal, lightDir), 0.0) * 0.5;

	// lightingColor = vec3(1.0, 1.0, 1.0) * (lightAngle + 0.25);

	light = 1.0;
	if(aNormal.y == 0.0) light = 0.8;
	if(aNormal.y == -1.0) light = 0.5;

	gl_Position = u_projection * u_view * fragPos;
}