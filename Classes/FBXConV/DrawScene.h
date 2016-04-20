#pragma once
#include "GlFunctions.h"

void InitializeLights(const FbxScene* pScene, const FbxTime & pTime, FbxPose* pPose = NULL);

void DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
	FbxAMatrix& pParentGlobalPosition,
	FbxPose* pPose, ShadingMode pShadingMode);

