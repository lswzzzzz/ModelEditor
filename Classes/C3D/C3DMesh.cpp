#include "C3DMesh.h"


C3DMesh::C3DMesh(void)
{
	m_bAniPlaying = false;
}

C3DMesh::~C3DMesh(void)
{
	DeleteAllSubMesh();
}

void C3DMesh::AddSubMesh(C3DSubMesh* pSubMesh)
{
	m_SubMeshVec.push_back(pSubMesh);
	AddChild(pSubMesh);
}

//删除所有子模型
void C3DMesh::DeleteAllSubMesh()
{
	std::vector<C3DSubMesh*>::iterator tIter;
	for(tIter=m_SubMeshVec.begin(); tIter!=m_SubMeshVec.end();tIter++){
		delete (*tIter);
		DelNode(*tIter);
	}
	m_SubMeshVec.clear();
}

//取得子模型数量
int C3DMesh::GetSubMeshCount()
{
	return m_SubMeshVec.size();
}

//取得指定的子模型
C3DSubMesh* C3DMesh::GetSubMesh(int vIndex)
{
	return m_SubMeshVec[vIndex];
}

C3DSubMesh* C3DMesh::GetSubMesh(const char* szSubMesh)
{
	std::vector<C3DSubMesh*>::iterator tIter;
	for(tIter=m_SubMeshVec.begin();tIter!=m_SubMeshVec.end();tIter++){
		if(0 == strcmp((*tIter)->GetName(), szSubMesh)){
			return (*tIter);
		}
	}
	return NULL;
}

//从文件中读取并创建模型
bool C3DMesh::LoadMeshFromFile(const char* szFileName)
{
	DeleteAllSubMesh();
	std::string  fullpath = FileUtils::getInstance()->fullPathForFilename(szFileName);
	FILE* pFile = fopen(fullpath.c_str(), "rb");
	if(pFile){
		int meshCount;
		fread(&meshCount, sizeof(int), 1, pFile);
		for (int n = 0; n < meshCount; n++){
			stMeshHeader tHeader;
			fread(&tHeader, sizeof(stMeshHeader), 1, pFile);
			for (int i = 0; i < tHeader.m_SubMeshCount; i++)
			{
				C3DSubMesh*	pSubMesh = new C3DSubMesh();
				if (false == pSubMesh->LoadMeshFromFile(pFile))
				{
					return false;
				}
				AddSubMesh(pSubMesh);
			}
		}
		fclose(pFile);
		return true;
	}
	return false;
}

bool C3DMesh::WriteMeshToFile(const char* szFileName)
{
	FILE* pFile = fopen(szFileName, "wb");
	if(pFile){
		stMeshHeader tHeader;
		strcpy(tHeader.m_MeshName, GetName());

		tHeader.m_SubMeshCount = m_SubMeshVec.size();
		fwrite(&tHeader, sizeof(stMeshHeader), 1, pFile);

		std::vector<C3DSubMesh*>::iterator tIter;
		for(tIter=m_SubMeshVec.begin();tIter!=m_SubMeshVec.end();tIter++){
			(*tIter)->WriteMeshToFile(pFile);
		}
		fclose(pFile);
		return true;
	}
	return false;
}

//从文件中读取骨骼动画
bool	C3DMesh::LoadSkinAniFromFile(const char* szFileName)
{
	std::string  fullpath = FileUtils::getInstance()->fullPathForFilename(szFileName);
	FILE*	pFile = fopen(fullpath.c_str(), "rb");
	if (pFile)
	{
		stSkinAniHeader	tHeader;
		fread(&tHeader, sizeof(stSkinAniHeader), 1, pFile);
		m_BoneNameVec.clear();
		char    szBoneName[64];
		for (int b = 0; b < tHeader.BoneCount; b++)
		{
			fread(szBoneName, sizeof(szBoneName), 1, pFile);
			string strBone = szBoneName;
			m_BoneNameVec.push_back(strBone);
		}
		for (int f = tHeader.BeginFrame; f <= tHeader.EndFrame; f++)
		{
			stSkinFrame	tNewSkinFrame;
			tNewSkinFrame.m_FrameID = f - tHeader.BeginFrame;
			for (int b = 0; b < tHeader.BoneCount; b++)
			{
				//矩阵
				Mat4 tBoneMatrix;
				fread(&tBoneMatrix, sizeof(Mat4), 1, pFile);
				tNewSkinFrame.m_BoneMatrixVec.push_back(tBoneMatrix);
			}
			m_AniFrameVec.push_back(tNewSkinFrame);
		}
		return true;
	}
	return false;
}

//播放动作
void	C3DMesh::PlaySkinAni()
{
	m_bAniPlaying = true;
	gettimeofday(&m_StartTime, 0);
}

//更新
void	C3DMesh::Update()
{
	if (m_bAniPlaying && false == m_AniFrameVec.empty() && false == m_SubMeshVec.empty())
	{
		timeval t;
		gettimeofday(&t, 0);
		double  delay = 1000.0f * (t.tv_sec - m_StartTime.tv_sec) + 0.001f * (t.tv_usec - m_StartTime.tv_usec);
		int     nMS = delay / 30.0f;
		//数量
		int		nAniFrameCount = m_AniFrameVec.size();
		nMS = nMS % nAniFrameCount;

		//设置当前帧的所有骨骼的变换矩阵
		int	   nBoneMatrixCount = m_AniFrameVec[nMS].m_BoneMatrixVec.size();
		vector<C3DSubMesh*>::iterator tIter;
		for (tIter = m_SubMeshVec.begin(); tIter != m_SubMeshVec.end(); tIter++)
		{
			(*tIter)->setBoneMatrixArray(&m_AniFrameVec[nMS].m_BoneMatrixVec.front(), nBoneMatrixCount);
		}
	}
}

bool C3DMesh::LoadMeshFromC3T(const char* c3t)
{
	C3DSubMesh*	pSubMesh = new C3DSubMesh();
	bool result = pSubMesh->LoadMeshFromC3T(c3t);
	AddSubMesh(pSubMesh);
	return result;
}

bool C3DMesh::LoadAniFromC3T(const char* c3t)
{
	ssize_t size = 0;
	std::string path = FileUtils::getInstance()->fullPathForFilename(c3t);
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	std::string load_str = std::string((const char*)titlech, size);
	rapidjson::Document document;
	document.Parse<0>(load_str.c_str());
	if (document.HasParseError() || !document.IsObject()){
		return false;
	}
	rapidjson::Value& meshs = document["meshes"];
	rapidjson::Value& materials = document["materials"];
	rapidjson::Value& nodes = document["nodes"];
	rapidjson::Value& animations = document["animations"];
	rapidjson::SizeType index = 0;
	rapidjson::Value& bones = animations[index]["bones"];
	for (int i = 0; i < bones.Capacity(); i++){
		stC3TFrames frames;
		std::string str = bones[i]["boneId"].GetString();
		memcpy(frames.Id, str.c_str(), str.size());
		rapidjson::Value& keyframes = bones[i]["keyframes"];
		for (int v = 0; v < keyframes.Capacity(); v++){
			rapidjson::Value& keyframe = keyframes[v];
			stC3TFrame frame;
			frame.keytime = keyframe["keytime"].GetDouble();
			rapidjson::Value& translation = keyframe["translation"];
			rapidjson::Value& scale = keyframe["scale"];
			rapidjson::Value& rotation = keyframe["rotation"];
			
			if (keyframe.HasMember("translation")){
				int index = 0;
				frame.translation = Vec3(translation[index++].GetDouble(), translation[index++].GetDouble(), translation[index++].GetDouble());
			}
			if (keyframe.HasMember("scale")){
				index = 0;
				frame.scale = Vec3(scale[index++].GetDouble(), scale[index++].GetDouble(), scale[index++].GetDouble());
			}
			if (keyframe.HasMember("rotation")){
				index = 0;
				frame.rotation = Vec4(rotation[index++].GetDouble(), rotation[index++].GetDouble(), rotation[index++].GetDouble(), rotation[index++].GetDouble());
			}
			frames.vec.push_back(frame);
		}
		m_C3TFrames.push_back(frames);
	}
	return true;
}