attribute vec3 a_position;
attribute vec2 a_texCoord;
attribute vec3 a_color;

varying vec2 uvVarying;
varying vec3 colorVarying;

void main()
{
	gl_Position = CC_MVPMatrix * vec4(a_position, 1.0);
	uvVarying = a_texCoord;
	colorVarying = a_color;
}
