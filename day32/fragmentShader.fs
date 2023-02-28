#version 330 core

#define TWO_PI 6.28318530718

out vec4 FragColor;

in vec3 FragPos;

vec3 hsb2rgb(vec3 c)
{
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0,0.0,1.0);
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix( vec3(1.0), rgb, c.y);
}

void main()
{
    vec3 color = vec3(1.0);
    color = hsb2rgb(vec3((atan(FragPos.x,FragPos.z)/TWO_PI) +0.5,1,1));
    FragColor = vec4(color,1.0);
}

