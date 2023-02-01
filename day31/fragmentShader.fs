#version 330 core

out vec4 FragColor;

in vec3 FragPos;

void main()
{
	//FragColor = vec4(FragPos.z, FragPos.y, FragPos.x, 1.0f);

	//FragColor = vec4((FragPos.x + -FragPos.z + FragPos.y) + 0.5f, (-FragPos.x + FragPos.z + FragPos.y) + 0.5f, (-FragPos.x + -FragPos.z + FragPos.y) + 0.5f, 1.0f);

	
	//FragColor = vec4(-FragPos.y + 1, FragPos.y + 1, 0.0f, 1.0f);
	FragColor = vec4(-FragPos.x + -FragPos.y + 1, FragPos.x + FragPos.y + 1, 0.0f, 1.0f);
	//FragColor = vec4(-FragPos.x + 1, FragPos.x + 1, 0.0f, 1.0f);
}

