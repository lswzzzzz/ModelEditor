#pragma once
#include <fbxsdk.h>
#include "GlFunctions.h"
#include "cocos2d.h"

using namespace cocos2d;

class SceneContext
{
public:
	enum Status
	{
		UNLOADED,               // Unload file or load failure;
		MUST_BE_LOADED,         // Ready for loading file;
		MUST_BE_REFRESHED,      // Something changed and redraw needed;
		REFRESHED               // No redraw needed.
	};

	// Initialize with a .FBX, .DAE or .OBJ file name and current window size.
	SceneContext(const char * pFileName, bool pSupportVBO);
	~SceneContext();
	Status GetStatus()const { return mStatus; };


	bool OnDisplay();
	bool LoadFile();
	void OnTimerClick() const;
	void DisplayCallBack();
public:
	enum CameraZoomMode
	{
		ZOOM_FOCAL_LENGTH,
		ZOOM_POSITION
	};

private:
	void DisplayGrid(const FbxAMatrix & pTransform);
	
private:
	enum CameraStatus
	{
		CAMERA_NOTHING,
		CAMERA_ORBIT,
		CAMERA_ZOOM,
		CAMERA_PAN
	};

	const char * mFileName;
	mutable Status mStatus;

	FbxManager * mSdkManager;
	FbxScene * mScene;
	FbxImporter * mImporter;
	FbxAnimLayer * mCurrentAnimLayer;
	FbxNode * mSelectedNode;

	int mPoseIndex;
	FbxArray<FbxString*> mAnimStackNameArray;
	FbxArray<FbxNode*> mCameraArray;
	FbxArray<FbxPose*> mPoseArray;

	mutable FbxTime mFrameTime, mStart, mStop, mCurrentTime;
	mutable FbxTime mCache_Start, mCache_Stop;

	// Data for camera manipulation
	mutable int mLastX, mLastY;
	mutable FbxVector4 mCamPosition, mCamCenter;
	mutable double mRoll;
	mutable CameraStatus mCameraStatus;

	bool mPause;
	ShadingMode mShadingMode;
	bool mSupportVBO;

	//camera zoom mode
	CameraZoomMode mCameraZoomMode;
};

