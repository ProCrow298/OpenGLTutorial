#version 330 core
out vec4 FragColor;

uniform  vec4 ourColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoord) * ourColor;
} 

