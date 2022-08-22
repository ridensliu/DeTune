// fragment shader

#version 150

uniform sampler2DRect tex0;
uniform float bluecolor;
uniform float redcolor;
uniform float greencolor;

in vec3 cusNormal;
in vec2 varyingtexcoord;
in vec4 cusColor;

out vec4 outputColor;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.

    float windowWidth = 1024.0;
    float windowHeight = 768.0;

    float r = gl_FragCoord.x / windowWidth * redcolor / 255.;
    float g = gl_FragCoord.y / windowHeight * greencolor / 255.;
    float b = 1.0 * bluecolor / 255.;
    float a = 1.0;

    outputColor = cusColor;
}
