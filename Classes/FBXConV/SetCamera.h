#pragma once

void SetCamera(FbxScene* pScene,
	FbxTime& pTime,
	FbxAnimLayer* pAnimLayer,
	const FbxArray<FbxNode*>& pCameraArray,
	int pWindowWidth, int pWindowHeight);

FbxCamera* GetCurrentCamera(FbxScene* pScene);

void CameraZoom(FbxScene* pScene, int pZoomDepth, int pZoomMode);

void CameraOrbit(FbxScene* pScene, FbxVector4 lOrigCamPos, double OrigRoll, int dX, int dY);

void CameraPan(FbxScene* pScene, FbxVector4 lOrigCamPos, FbxVector4 lOrigCamCenter,
	double OrigRoll, int dX, int dY);
