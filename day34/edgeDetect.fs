

#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

// texture samplers
uniform sampler2D texture1;

float x = 0;

void main()
{
	if(FragPos.x > 100)
	{
		x = 1;
	}
	else
	{
		x = 0;
	}
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(x,0,0,1);
}

//#version 330 core
//out vec4 FragColor;
//
//in vec3 FragPos;  
//in vec3 Normal;  
//in vec2 TexCoords;
//
//// texture samplers
//uniform sampler2D texture1;
//
//void main()
//{
//	// linearly interpolate between both textures (80% container, 20% awesomeface)
//	FragColor = texture(texture1, TexCoords);
//}

