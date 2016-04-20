#pragma once
#include "cocos2d.h"
#include "C3DShape.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

USING_NS_CC;
using namespace rapidjson;

struct stSubMeshHeader
{
	//ģ������
	char m_SubMeshName[32];
	//��ͼ�ļ�
	char m_Texture[32];
	//��������
	unsigned int m_VertexCount;
	//��������
	unsigned int m_IndexCount;
	unsigned int	m_WeightCount;		//��Ƥ��Ϣ�ṹ������
	//ͼ����Ⱦ��ʽ
	PrimitiveType m_PrimitiveType;
	IndexFormat m_IndexFormat;
};

class C3DSubMesh : public C3DShape
{
public:
	C3DSubMesh(void);
	~C3DSubMesh(void);

public:
	//���ļ��ж�ȡ������ģ��
	bool LoadMeshFromFile(FILE* pFile);

	bool LoadMeshFromC3T(const char* c3t);
	void	RecursiveNode(rapidjson::Value& node, bool isskeleton = false);
	//��ģ�ͱ��浽�ļ���
	bool WriteMeshToFile(FILE* pFile);

private:
	std::vector<Mat4> translationVec;;
	std::vector<std::string> idVec;
};

