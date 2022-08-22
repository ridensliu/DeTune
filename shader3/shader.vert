
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

float rand(vec2 n) { 
	return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 p){
	vec2 ip = floor(p);
	vec2 u = fract(p);
	u = u*u*(3.0-2.0*u);
	
	float res = mix(
		mix(rand(ip),rand(ip+vec2(1.0,0.0)),u.x),
		mix(rand(ip+vec2(0.0,1.0)),rand(ip+vec2(1.0,1.0)),u.x),u.y);
	return res*res ;
}

void main(){
    cusNormal = normal;
    varyingtexcoord = vec2(position.x / 1024.0, position.y / 1024.0);
    cusColor = color;

    vec4 spos;
    float r1 = (position.z - 700.0) * noise(varyingtexcoord * 17.0 + vec2(time * 2.0, 0.0)) *  noisevalue + 700.0;
    float r = r1 * 0.3;
    spos.x = r * sin(position.x / 1024.0 * PI * 2.0) * cos(position.y / 1024.0 * PI * 2.0);
    spos.y = r * sin(position.x / 1024.0 * PI * 2.0) * sin(position.y / 1024.0 * PI * 2.0);
    spos.z = r * cos(position.x / 1024.0 * PI * 2.0);
    // spos.x = sin(position.x / 1024.0 * PI * 2.0) * 500.0;
    // spos.y = cos(position.x / 1024.0 * PI * 2.0) * 500.0;
    // spos.z = position.y;
    spos.w = position.w;

    //X rotate
    float cosRY = cos(time * 25./360. * PI * 2.);
    float sinRY = sin(time * 25./360. * PI * 2.);

    float tempz = spos.z;
    float tempx = spos.x;

    spos.x = (tempx * cosRY) + (tempz * sinRY);
    spos.z = (tempx * -sinRY) + (tempz * cosRY);

    //Y rotate
    // float cosRX = cos(time * 15./360. * PI * 2.);
    // float sinRX = sin(time * 15./360. * PI * 2.);

    // tempz = spos.z;
    // float tempy = spos.y;

    // spos.y = (tempy * cosRX) + (tempz * sinRX);
    // spos.z = (tempx * -sinRX) + (tempz * cosRX);

    gl_Position = modelViewProjectionMatrix * spos;
}
