#pragma once
#include "cocos2d.h"
#include "C3DNode.h"

USING_NS_CC;


//顶点类型
enum VertexChannelType{
	VERTEX_ATTRIB_POSITION,
	VERTEX_ATTRIB_COLOR,
	VERTEX_ATTRIB_TEX_COORD,
	VERTEX_ATTRIB_NORMAL,
	VERTEX_ATTRIB_BLEND_WEIGHT,
	VERTEX_ATTRIB_BLEND_INDEX,
	VERTEX_ATTRIB_MAX,
	VERTEX_ATTRIB_TEX_COORDS = VERTEX_ATTRIB_TEX_COORD,
};

//索引类型
enum IndexFormat
{
	INDEX8 = GL_UNSIGNED_BYTE,
	INDEX16 = GL_UNSIGNED_SHORT,

};

//图形绘制类型
enum PrimitiveType
{
	PT_POINTS = GL_POINTS,
	PT_TRIANGLES = GL_TRIANGLES,
	PT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	PT_LINES = GL_LINES,
	PT_LINE_STRIP = GL_LINE_STRIP,
};

//顶点受骨骼权重影响信息
struct stVertex_BoneWeight
{
	int mVertexIndex;	//顶点索引
	int mBoneIndex;		//骨骼索引
	float mWeight;		//影响权重
};

struct stShapeVertices
{
	Vec3		Position;
	Vec3		Normal;
	Vec3		Color;		
	Vec2        TexUV;
};

struct stSkinVertices
{
	Vec3 Position; // 位置
	Vec3 Normal;   // 法线
	Vec4 Color; // 色彩
	Vec2 TexUV; // uv 坐标
	Vec4 Blend; // 顶点绑定在最多四根骨骼上, 这里指示对应的四根骨骼的混合权重值
	Vec4 Indices; // 顶点绑定在最多四根骨骼上, 这里指示哪四根, 即骨骼的索引值
};

class C3DShape : public C3DNode
{
public:
	enum C3DShape_Type{
		SHAPE_USERDEFINE,
		SHAPE_POINTS,
		SHAPE_LINE,
		SHAPE_TRIANGLES,
		SHAPE_RECTANGLE,
		SHAPE_SPHERE,
		SHAPE_3DMAX_OBJ,
		SHAPE_3DMAX_MODEL,
		SHAPE_3DMAX_BONEANIMATION,
		SHAPE_C3T,
	};
	C3DShape(void);
	~C3DShape(void);
	public:
	//创建Shader
	virtual void	BuildShader();
	//渲染
	virtual void	Render();

	virtual void	Render_ShapeVertices();
	virtual void	Render_SkinVertices();
	//创建点
	void	CreatePoints();
	//创建线
	void	CreateLines();
	//创建三角形
	void	CreateTriangles();
	//创建四边形
	void    CreateRectangle();
	//创建球
	void	CreateSphere();

	void	CreateObj(const char* obj);

	void CreateSkinLines();

	int draw_type;

	void SetTexture(const char* texture, bool repeat = false);
	void SetCubeTexture(const char* szTextureFileName);
	const char* GetTextureFileName();

	//设置骨骼矩阵数组
	void setBoneMatrixArray(Mat4* pBoneMatrixArray, int nBoneMatrixCount);

	const BlendFunc& getBlendFunc();
	void setBlendFunc(const BlendFunc& func);
protected:
	//释放
	virtual void	Release();
protected:
	//图形渲染方式
	PrimitiveType		m_PrimitiveType;
	//索引格式
	IndexFormat			m_IndexFormat;
	//顶点数组
	stShapeVertices*	m_VertexArray;
	//骨骼动画数组
	stSkinVertices*     m_SkinVertexArray;

	//索引数组
	GLushort*			m_IndiceArray;
	//VB
	GLuint				m_VertexBuffer;
	//IB
	GLuint				m_IndexBuffer;
	//顶点数量
	int					m_VertexCount;
	//索引数量
	int					m_IndexCount;
	//所用的Shader
	GLProgram*			m_ShaderProgram;
	Texture2D*			 m_Texture;
	std::string			m_TextureFileName;
	GLuint vao;
	bool				isTextureDDS;
	GLuint				TextureDDS;

	BlendFunc			m_BlendFunc;
	bool				is_DrawColor;
	bool				is_Draw_Texture;

	//骨骼矩阵数组
	Mat4*				m_BoneMatrixArray;
	//骨骼数量
	int					m_BoneMatrixCount;

};

