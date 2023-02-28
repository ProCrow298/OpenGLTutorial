#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D image;
uniform vec2 resolution;

mat3 sobel_x = mat3 (
     1.0,  2.0,  1.0,
     0.0,  0.0,  0.0,
    -1.0, -2.0, -1.0 
);

mat3 sobel_y = mat3 (
   1.0,  0.0, -1.0,
   2.0,  0.0, -2.0,
   1.0,  0.0, -1.0 
);

void main() 
{
    vec2 tex_coord = gl_FragCoord.xy;

    float Gx = 0.0;
    float Gy = 0.0;

    for (int i = -1; i <= 1; i++) 
    {
        for (int j = -1; j <= 1; j++) 
        {
            vec2 offset = vec2(i,j);
            vec4 sample = texelFetch(image, ivec2(tex_coord + offset), 0);

            Gx += length(sample) * sobel_x[i+1][j+1];
            Gy += length(sample) * sobel_y[i+1][j+1];
        }
    }
    float sobel = sqrt(pow(Gx, 2) + pow(Gy, 2));


    //FragColor = texture(image, TexCoord);
    FragColor = vec4(vec3(sobel), 1.0);
}