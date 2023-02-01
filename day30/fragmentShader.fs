#version 330 core

out vec4 FragColor;

in vec3 FragPos;

void main()
{
	FragColor = vec4(FragPos.y + 1,FragPos.y - .1,FragPos.y - .1, 1.0f);
}

