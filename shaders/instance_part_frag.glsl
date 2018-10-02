#version 400 core

// interpolated values from vertex shader
in vec2 TexCoords;
in vec4 ParticleColor;

out vec4 color;

uniform sampler2D TexSampler;

void main()
{
	color = texture(TexSampler, TexCoords) * ParticleColor;
}