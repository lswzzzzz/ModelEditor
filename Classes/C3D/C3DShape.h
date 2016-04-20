#pragma once
#include "cocos2d.h"
#include "C3DNode.h"

USING_NS_CC;


//��������
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

//��������
enum IndexFormat
{
	INDEX8 = GL_UNSIGNED_BYTE,
	INDEX16 = GL_UNSIGNED_SHORT,

};

//ͼ�λ�������
enum PrimitiveType
{
	PT_POINTS = GL_POINTS,
	PT_TRIANGLES = GL_TRIANGLES,
	PT_TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
	PT_LINES = GL_LINES,
	PT_LINE_STRIP = GL_LINE_STRIP,
};

//�����ܹ���Ȩ��Ӱ����Ϣ
struct stVertex_BoneWeight
{
	int mVertexIndex;	//��������
	int mBoneIndex;		//��������
	float mWeight;		//Ӱ��Ȩ��
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
	Vec3 Position; // λ��
	Vec3 Normal;   // ����
	Vec4 Color; // ɫ��
	Vec2 TexUV; // uv ����
	Vec4 Blend; // �����������ĸ�������, ����ָʾ��Ӧ���ĸ������Ļ��Ȩ��ֵ
	Vec4 Indices; // �����������ĸ�������, ����ָʾ���ĸ�, ������������ֵ
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
	//����Shader
	virtual void	BuildShader();
	//��Ⱦ
	virtual void	Render();

	virtual void	Render_ShapeVertices();
	virtual void	Render_SkinVertices();
	//������
	void	CreatePoints();
	//������
	void	CreateLines();
	//����������
	void	CreateTriangles();
	//�����ı���
	void    CreateRectangle();
	//������
	void	CreateSphere();

	void	CreateObj(const char* obj);

	void CreateSkinLines();

	int draw_type;

	void SetTexture(const char* texture, bool repeat = false);
	void SetCubeTexture(const char* szTextureFileName);
	const char* GetTextureFileName();

	//���ù�����������
	void setBoneMatrixArray(Mat4* pBoneMatrixArray, int nBoneMatrixCount);

	const BlendFunc& getBlendFunc();
	void setBlendFunc(const BlendFunc& func);
protected:
	//�ͷ�
	virtual void	Release();
protected:
	//ͼ����Ⱦ��ʽ
	PrimitiveType		m_PrimitiveType;
	//������ʽ
	IndexFormat			m_IndexFormat;
	//��������
	stShapeVertices*	m_VertexArray;
	//������������
	stSkinVertices*     m_SkinVertexArray;

	//��������
	GLushort*			m_IndiceArray;
	//VB
	GLuint				m_VertexBuffer;
	//IB
	GLuint				m_IndexBuffer;
	//��������
	int					m_VertexCount;
	//��������
	int					m_IndexCount;
	//���õ�Shader
	GLProgram*			m_ShaderProgram;
	Texture2D*			 m_Texture;
	std::string			m_TextureFileName;
	GLuint vao;
	bool				isTextureDDS;
	GLuint				TextureDDS;

	BlendFunc			m_BlendFunc;
	bool				is_DrawColor;
	bool				is_Draw_Texture;

	//������������
	Mat4*				m_BoneMatrixArray;
	//��������
	int					m_BoneMatrixCount;

};

