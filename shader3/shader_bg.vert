
// vertex shader

#version 150
#define PI 3.1415926538

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform float noisevalue;

in vec4 position;
in vec3 normal;
in vec4 color;

out vec3 cusNormal;
out vec2 varyingtexcoord;
out vec4 cusColor;

void main(){
    cusNormal = normal;
    cusColor = color;
    varyingtexcoord = vec2(0.0);
    
    gl_Position = modelViewProjectionMatrix * position;
}
