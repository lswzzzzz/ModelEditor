#pragma once
#include "cocos2d.h"
#include "fbxsdk.h"
#include "C3DShape.h"

using namespace cocos2d;
using namespace std;

struct ModelBoneWeight
{
	float boneIndex[4];
	float boneWeight[4];
};

struct ModelMesh
{
	std::string nodeName;
	std::string materialName;
	std::vector<stSkinVertices> vertexList;
	std::vector<int> indexList;
	Mat4 invMeshBaseposeMatrix;
	std::vector<std::string> boneNodeNameList;
	std::vector<Mat4> invBoneBaseposeMatrixList;
};

class Exporter
{
public:
	Exporter(const char* filename);
	~Exporter();
	bool LoadFile();
	void RecursiveRoot();
	void RecursiveNode(FbxNode* node);
	void GetCurNode(FbxNode* node);
	void ReadMesh(FbxMesh* mesh);
	void SaveMeshToFile(const char* file);
	void LoadMeshFromFile(const char*file);
	ModelMesh ParseMesh(FbxMesh* mesh);
	bool LoadBone(std::string fbx_file);
private:
	const char* mFileName;
	FbxManager* mSdkManager;
	FbxScene* mScene;
	FbxImporter * mImporter;
	FbxAnimLayer* mCurrentAnimLayer;

	FbxArray<FbxString*> mAnimStackNameArray;
	FbxArray<FbxNode*> mCameraArray;
	FbxArray<FbxPose*> mPoseArray;

	mutable FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	mutable FbxTime mCache_Start, mCache_Stop;
	FILE* mFile;

	std::vector<ModelMesh> m_meshList;
};

