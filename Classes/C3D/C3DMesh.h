#pragma once
#include "cocos2d.h"
#include "C3DNode.h"
#include "C3DSubMesh.h"

using namespace cocos2d;
using namespace std;

//头信息
struct    stMeshHeader
{
	char			m_MeshName[32];		//模型名称
	int 			m_SubMeshCount;		//子模型数量
};

struct stC3TFrame{
	float keytime;
	Vec3 translation;
	Vec3 scale;
	Vec4 rotation;
};

struct stC3TFrames{
	char Id[60];
	std::vector<stC3TFrame> vec;
	stC3TFrames(){
		memset(Id, 0, 60);
	}
};

struct		stSkinAniHeader
{
	char	Name[60];			// 名称
	int		BoneCount;			// 骨骼数量
	int		BeginFrame;			// 开始帧
	int		EndFrame;			// 结束帧
	int		FrameRate;			// 帧速度

	stSkinAniHeader()
	{
		Name[0] = '\0';
		BoneCount = 0;
		BeginFrame = 0;
		EndFrame = 0;
		FrameRate = 0;

	}
};

struct		stSkinFrame
{
	int				m_FrameID;
	vector<Mat4>	m_BoneMatrixVec;
};

class C3DMesh : public C3DNode
{
public:
	C3DMesh(void);
	~C3DMesh(void);
public:
	//更新
	virtual	void	Update();
public:
	//增加子模型
	void AddSubMesh(C3DSubMesh* pSubMesh);
	//删除所有子模型
	void DeleteAllSubMesh();
	//取得子模型数量
	int GetSubMeshCount();
	//取得指定的子模型
	C3DSubMesh* GetSubMesh(int vIndex);
	C3DSubMesh* GetSubMesh(const char* szSubMesh);
	//从文件中读取并创建模型
	bool LoadMeshFromFile(const char* szFileName);
	bool WriteMeshToFile(const char* szFileName);

	bool LoadMeshFromC3T(const char* c3t);
	bool LoadAniFromC3T(const char* c3t);

	//从文件中读取骨骼动画
	bool    LoadSkinAniFromFile(const char* szFileName);
	//播放动作
	void    PlaySkinAni();
private:
	//子模型容器
	std::vector<C3DSubMesh*> m_SubMeshVec;
	//骨骼名称容器
	vector<string>		m_BoneNameVec;
	//骨骼动画
	vector<stSkinFrame>	m_AniFrameVec;
	//开始时间
	timeval				m_StartTime;
	//动作播放
	bool				m_bAniPlaying;
	vector<stC3TFrames> m_C3TFrames;
};

