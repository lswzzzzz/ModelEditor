#pragma once
#include "fbxsdk.h"

void FillPoseArray(FbxScene* pScene, FbxArray<FbxPose*>& pPoseArray);
void PreparePointCacheData(FbxScene* pScene, FbxTime &pCache_Start, FbxTime &pCache_Stop);
void FillCameraArrayRecursive(FbxNode* pNode, FbxArray<FbxNode*>& pCameraArray);
void LoadCacheRecursive(FbxNode * pNode, FbxAnimLayer * pAnimLayer, bool pSupportVBO);
bool LoadTextureFromFile(const FbxString & pFilePath, unsigned int & pTextureObject);
void LoadCacheRecursive(FbxScene * pScene, FbxAnimLayer * pAnimLayer, const char * pFbxFileName, bool pSupportVBO);
void FillCameraArray(FbxScene* pScene, FbxArray<FbxNode*>& pCameraArray);
void UnloadCacheRecursive(FbxNode * pNode);
void UnloadCacheRecursive(FbxScene * pScene);

