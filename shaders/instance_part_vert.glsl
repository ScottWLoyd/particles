#version 400 core
layout (location = 0) in vec3 vertex; 
layout (location = 1) in vec4 pos_size;
layout (location = 2) in vec4 color;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;

void main()
{
	float particle_size = pos_size.w;

	gl_Position = pos_size.xyz + projection * vec4((vertex.xy * particle_size), 0.0, 1.0);
	TexCoords = vertex.xy + vec2(0.5, 0.5);
	ParticleColor = color;
}
