varying vec2 uvVarying;
varying vec3 colorVarying;

uniform sampler2D myTextureSampler;
uniform int is_drawColor;
uniform int is_drawTexture;

void main()
{
	vec4 g_color;
	vec4 g_texture;
	if(is_drawColor){
		g_color = vec4(colorVarying, 1);
	}else{
		g_color = vec4(1, 1, 1, 1);
	}
	if(is_drawTexture){
		g_texture = texture2D(myTextureSampler, uvVarying);
	}else{
		g_texture = vec4(1, 1, 1, 1);
	}
	gl_FragColor = g_texture * g_color;
}