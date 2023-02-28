//#version 330 core
//
//in vec3 FragPos;
//
//out vec4 FragColor;
//
//uniform float u_time;
//
//vec2 random2(vec2 st){
//    st = vec2( dot(st,vec2(127.1,311.7)),
//              dot(st,vec2(269.5,183.3)) );
//    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
//}
//
//float noise(vec2 st) {
//    vec2 i = floor(st);
//    vec2 f = fract(st);
//
//    vec2 u = f*f*(3.0-2.0*f);
//
//    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
//                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
//                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
//                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
//}
//
//void main() {
//    vec2 st = gl_FragCoord.xy / 100; 
//  //vec2 st = FragPos.xy / 100; 
//
//    float t = 1.0;
//    // Uncomment to animate
//    // t = abs(1.0-sin(u_time*.1))*5.;
//    // Comment and uncomment the following lines:
//    st += noise(st*2.)*t; // Animate the coordinate space
//    color = vec3(1.) * smoothstep(.18,.2,noise(st)); // Big black drops
//    color += smoothstep(.15,.2,noise(st*10.)); // Black splatter
//    color -= smoothstep(.35,.4,noise(st*10.)); // Holes on splatter
//
//    FragColor = vec4(1.-color,1.0);
//}

    

//Full color spectrum Fragment shader 
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


