#version 130

precision mediump float;

in vec2 vuv;

uniform sampler2D texture;

void main(void)
{
	gl_FragColor = texture2D(texture, vuv);
}
