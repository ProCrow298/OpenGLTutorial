#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	
	//gl_Position = vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	
	gl_Position = vec4(projection * vec4(aPos, 1.0));
	//test = aTexCoord;
}