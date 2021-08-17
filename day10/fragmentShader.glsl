#version 330 core
out vec4 FragColor;
  
in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  
uniform float foo;
void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);  
} 