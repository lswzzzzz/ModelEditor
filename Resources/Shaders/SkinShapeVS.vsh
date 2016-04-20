attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;
attribute vec4 a_blendWeight;
attribute vec4 a_blendIndex;
#ifdef GL_ES
varying mediump vec2 v_texCoord;
#else
varying vec2 v_texCoord;
#endif
const int MAX_MATRIXSIZE = 80;
uniform mat4 u_matrixPalette[MAX_MATRIXSIZE];
void main()
{
	//取得骨骼顶点结构中的四根绑定骨骼的索引
	int nBlendIndex1 = int(a_blendIndex.x);
	int nBlendIndex2 = int(a_blendIndex.y);
	int nBlendIndex3 = int(a_blendIndex.z);
	int nBlendIndex4 = int(a_blendIndex.w);
	
	//用权重积加公式计算出最终的顶点位置。
	vec4 skinposition = vec4(0,0,0,0);
	if( a_blendWeight.x > 0.0 && nBlendIndex1 < MAX_MATRIXSIZE )
	{
		//第一根骨骼的矩阵x 相应权重得出受第一根骨骼影响的矩阵。
		mat4 matrixpalette = u_matrixPalette[nBlendIndex1] * a_blendWeight.x;
		//用结果矩阵x 原始顶点位置得出受第一根骨骼影响的位置。
		skinposition += matrixpalette * vec4(a_position, 1);
	}
	if( a_blendWeight.y > 0.0 && nBlendIndex2 < MAX_MATRIXSIZE )
	{
		mat4 matrixpalette = u_matrixPalette[nBlendIndex2] * a_blendWeight.y;
		skinposition += matrixpalette * vec4(a_position, 1);
	}
	if( a_blendWeight.z > 0.0 && nBlendIndex3 < MAX_MATRIXSIZE )
	{
		mat4 matrixpalette = u_matrixPalette[nBlendIndex3] * a_blendWeight.z;
		skinposition += matrixpalette * vec4(a_position, 1);
	}
	if( a_blendWeight.w > 0.0 && nBlendIndex4 < MAX_MATRIXSIZE )
	{
		mat4 matrixpalette = u_matrixPalette[nBlendIndex4] * a_blendWeight.w;
		skinposition += matrixpalette * vec4(a_position, 1);
	}
	v_texCoord = a_texCoord;
	//用最终的结果进行MVP 矩阵的转换。
	gl_Position = CC_MVPMatrix * skinposition;
};