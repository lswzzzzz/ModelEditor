#ifdef GL_ES
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif

uniform sampler2D myTextureSampler;
uniform int is_Draw_Texture;

void main()
{
	vec4 g_texture = vec4(1, 1, 1, 1);
	if(is_Draw_Texture){
		g_texture = texture2D(myTextureSampler, v_texCoord);
	}
	gl_FragColor = g_texture;
}