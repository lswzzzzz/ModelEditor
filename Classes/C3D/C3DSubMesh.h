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
	//模型名称
	char m_SubMeshName[32];
	//贴图文件
	char m_Texture[32];
	//顶点数量
	unsigned int m_VertexCount;
	//索引数量
	unsigned int m_IndexCount;
	unsigned int	m_WeightCount;		//蒙皮信息结构体数量
	//图形渲染方式
	PrimitiveType m_PrimitiveType;
	IndexFormat m_IndexFormat;
};

class C3DSubMesh : public C3DShape
{
public:
	C3DSubMesh(void);
	~C3DSubMesh(void);

public:
	//从文件中读取并创建模型
	bool LoadMeshFromFile(FILE* pFile);

	bool LoadMeshFromC3T(const char* c3t);
	void	RecursiveNode(rapidjson::Value& node, bool isskeleton = false);
	//将模型保存到文件中
	bool WriteMeshToFile(FILE* pFile);

private:
	std::vector<Mat4> translationVec;;
	std::vector<std::string> idVec;
};

