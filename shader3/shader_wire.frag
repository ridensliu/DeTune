// fragment shader

#version 150

uniform sampler2DRect superTex;
uniform float time;
uniform float wirevalue;

in vec3 cusNormal;
in vec2 varyingtexcoord;
in vec4 cusColor;

out vec4 outputColor;

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
	return res*res;
}

void main()
{
    // cusColor.rgb
    outputColor = vec4(vec3(1.0), wirevalue * noise(varyingtexcoord * 20.0 + vec2(time * 5.0, 0.0)));//texture(tex0, varyingtexcoord);
}
