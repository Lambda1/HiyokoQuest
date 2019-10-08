#version 130

precision mediump float;

in vec3 pos;
in vec2 uv;

out vec2 vuv;

void main(void)
{
	// UV
	vuv = uv;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(pos, 1.0);
}
