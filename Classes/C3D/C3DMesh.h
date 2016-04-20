#pragma once
#include "cocos2d.h"
#include "C3DNode.h"
#include "C3DSubMesh.h"

using namespace cocos2d;
using namespace std;

//ͷ��Ϣ
struct    stMeshHeader
{
	char			m_MeshName[32];		//ģ������
	int 			m_SubMeshCount;		//��ģ������
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
	char	Name[60];			// ����
	int		BoneCount;			// ��������
	int		BeginFrame;			// ��ʼ֡
	int		EndFrame;			// ����֡
	int		FrameRate;			// ֡�ٶ�

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
	//����
	virtual	void	Update();
public:
	//������ģ��
	void AddSubMesh(C3DSubMesh* pSubMesh);
	//ɾ��������ģ��
	void DeleteAllSubMesh();
	//ȡ����ģ������
	int GetSubMeshCount();
	//ȡ��ָ������ģ��
	C3DSubMesh* GetSubMesh(int vIndex);
	C3DSubMesh* GetSubMesh(const char* szSubMesh);
	//���ļ��ж�ȡ������ģ��
	bool LoadMeshFromFile(const char* szFileName);
	bool WriteMeshToFile(const char* szFileName);

	bool LoadMeshFromC3T(const char* c3t);
	bool LoadAniFromC3T(const char* c3t);

	//���ļ��ж�ȡ��������
	bool    LoadSkinAniFromFile(const char* szFileName);
	//���Ŷ���
	void    PlaySkinAni();
private:
	//��ģ������
	std::vector<C3DSubMesh*> m_SubMeshVec;
	//������������
	vector<string>		m_BoneNameVec;
	//��������
	vector<stSkinFrame>	m_AniFrameVec;
	//��ʼʱ��
	timeval				m_StartTime;
	//��������
	bool				m_bAniPlaying;
	vector<stC3TFrames> m_C3TFrames;
};

