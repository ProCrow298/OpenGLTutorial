#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D TextureSampler;
uniform vec2 u_resolution;
uniform float u_time;

void main()
{
    FragColor = texture(TextureSampler, texCoord);
}
