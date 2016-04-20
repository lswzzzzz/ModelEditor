#pragma once
#include <fbxsdk.h>
#include "cocos2d.h"

using namespace cocos2d;

enum ShadingMode
{
	SHADING_MODE_WIREFRAME,
	SHADING_MODE_SHADED,
};


void GlSetCameraPerspective(double pFieldOfViewY,
	double pAspect,
	double pNearPlane,
	double pFarPlane,
	FbxVector4& pEye,
	FbxVector4& pCenter,
	FbxVector4& pUp,
	double  pFilmOffsetX,
	double  pFilmOffsetY);
void GlSetCameraOrthogonal(double pLeftPlane,
	double pRightPlane,
	double pBottomPlane,
	double pTopPlane,
	double pNearPlane,
	double pFarPlane,
	FbxVector4& pEye,
	FbxVector4& pCenter,
	FbxVector4& pUp);

void GlDrawMarker(FbxAMatrix& pGlobalPosition);
void GlDrawLimbNode(FbxAMatrix& pGlobalBasePosition,
	FbxAMatrix& pGlobalEndPosition);
void GlDrawCamera(FbxAMatrix& pGlobalPosition,
	double pRoll);
void GlDrawCrossHair(FbxAMatrix& pGlobalPosition);


