#include "SceneContext.h"
#include "DrawScene.h"
#include "Common/Common.h"
#include "Global.h"
#include "SetCamera.h"
#include "SceneUtils.h"

SceneContext::SceneContext(const char * pFileName, bool pSupportVBO)
	:mFileName(pFileName), mStatus(UNLOADED),
	mSdkManager(NULL), mScene(NULL), mImporter(NULL), mCurrentAnimLayer(NULL), mSelectedNode(NULL),
	mPoseIndex(-1), mCameraStatus(CAMERA_NOTHING), mPause(false), mShadingMode(SHADING_MODE_SHADED),
	mSupportVBO(pSupportVBO), mCameraZoomMode(ZOOM_FOCAL_LENGTH)
{
	mCache_Start = FBXSDK_TIME_INFINITE;
	mCache_Stop = FBXSDK_TIME_MINUS_INFINITE;

	InitializeSdkObjects(mSdkManager, mScene);

	if (mSdkManager){
		int lFileFormat = -1;
		mImporter = FbxImporter::Create(mSdkManager, "");
		if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(mFileName, lFileFormat))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
		}

		if (mImporter->Initialize(mFileName, lFileFormat) == true){
			addConsoleInfo("导入模型文件%s,请稍等!", mFileName);
			mStatus = MUST_BE_LOADED;
		}
		else{
			addConsoleInfo("无法加载模型文件%s 错误数据返回为%s", mFileName, mImporter->GetStatus().GetErrorString());
		}
	}
	else{
		addConsoleInfo("无法创建FBX SDK Manager");
	}
}

SceneContext::~SceneContext()
{
	FbxArrayDelete(mAnimStackNameArray);
	
	if (mScene){
		UnloadCacheRecursive(mScene);
	}

	DestroySdkObjects(mSdkManager, true);
}

//循环播放
void SceneContext::OnTimerClick() const
{
	// Loop in the animation stack if not paused.
	if (mStop > mStart && !mPause)
	{
		// Set the scene status flag to refresh 
		// the scene in the next timer callback.
		mStatus = MUST_BE_REFRESHED;

		mCurrentTime += mFrameTime;

		if (mCurrentTime > mStop)
		{
			mCurrentTime = mStart;
		}
	}
	// Avoid displaying the same frame on 
	// and on if the animation stack has no length.
	else
	{
		// Set the scene status flag to avoid refreshing 
		// the scene in the next timer callback.
		mStatus = REFRESHED;
	}
}

void SceneContext::DisplayCallBack()
{
	if (GetStatus() == SceneContext::MUST_BE_REFRESHED){
		auto glView = dynamic_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView());
		
	}
	this->OnTimerClick();
}

bool SceneContext::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto glView = dynamic_cast<GLViewImpl*>(Director::getInstance()->getOpenGLView());
	glfwSwapBuffers(glView->getWindow());
	static bool ss = true;
	if (mStatus != UNLOADED && mStatus != MUST_BE_LOADED)
	{
		glPushAttrib(GL_ENABLE_BIT);
		glPushAttrib(GL_LIGHTING_BIT);
		glEnable(GL_DEPTH_TEST);
		// Draw the front face only, except for the texts and lights.
		glEnable(GL_CULL_FACE);

		// Set the view to the current camera settings.
		auto frameSize = Director::getInstance()->getVisibleSize();
		SetCamera(mScene, mCurrentTime, mCurrentAnimLayer, mCameraArray,
			frameSize.width, frameSize.height);

		FbxPose * lPose = NULL;
		if (mPoseIndex != -1)
		{
			lPose = mScene->GetPose(mPoseIndex);
		}

		// If one node is selected, draw it and its children.
		FbxAMatrix lDummyGlobalPosition;

		
		if (mSelectedNode && ss)
		{
			// Set the lighting before other things.
			InitializeLights(mScene, mCurrentTime, lPose);
			DrawNodeRecursive(mSelectedNode, mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, mShadingMode);
			DisplayGrid(lDummyGlobalPosition);
		}
		// Otherwise, draw the whole scene.
		else if (ss)
		{
			InitializeLights(mScene, mCurrentTime, lPose);
			DrawNodeRecursive(mScene->GetRootNode(), mCurrentTime, mCurrentAnimLayer, lDummyGlobalPosition, lPose, mShadingMode);
			DisplayGrid(lDummyGlobalPosition);
		}
		ss = false;
		glPopAttrib();
		glPopAttrib();
	}
	return true;
}

void SceneContext::DisplayGrid(const FbxAMatrix & pTransform)
{
	glPushMatrix();
	glMultMatrixd(pTransform);

	// Draw a grid 500*500
	glColor3f(0.3f, 0.3f, 0.3f);
	glLineWidth(1.0);
	const int hw = 500;
	const int step = 20;
	const int bigstep = 100;
	int       i;

	// Draw Grid
	for (i = -hw; i <= hw; i += step) {

		if (i % bigstep == 0) {
			glLineWidth(2.0);
		}
		else {
			glLineWidth(1.0);
		}
		glBegin(GL_LINES);
		glVertex3i(i, 0, -hw);
		glVertex3i(i, 0, hw);
		glEnd();
		glBegin(GL_LINES);
		glVertex3i(-hw, 0, i);
		glVertex3i(hw, 0, i);
		glEnd();

	}

	// Write some grid info
	const GLfloat zoffset = -2.f;
	const GLfloat xoffset = 1.f;
	for (i = -hw; i <= hw; i += bigstep)
	{

		FbxString scoord;
		int lCount;

		// Don't display origin
		//if (i == 0) continue;
		if (i == 0) {
			scoord = "0";
			lCount = (int)scoord.GetLen();
			glPushMatrix();
			glVertex3f(i + xoffset, 0, zoffset);
			glRotatef(-90, 1, 0, 0);

			glPopMatrix();

			continue;
		}

		// X coordinates
		scoord = "X: ";
		scoord += i;
		lCount = (int)scoord.GetLen();

		glPushMatrix();
		glTranslatef(i + xoffset, 0, zoffset);
		glRotatef(-90, 1, 0, 0);
		glPopMatrix();

		// Z coordinates
		scoord = "Z: ";
		scoord += i;
		lCount = (int)scoord.GetLen();

		glPushMatrix();
		glTranslatef(xoffset, 0, i + zoffset);
		glRotatef(-90, 1, 0, 0);
		glPopMatrix();

	}

	glPopMatrix();
}

bool SceneContext::LoadFile()
{
	bool lResult = false;
	// Make sure that the scene is ready to load.
	if (mStatus == MUST_BE_LOADED)
	{
		if (mImporter->Import(mScene) == true)
		{
			// Set the scene status flag to refresh 
			// the scene in the first timer callback.
			mStatus = MUST_BE_REFRESHED;

			// Convert Axis System to what is used in this example, if needed
			FbxAxisSystem SceneAxisSystem = mScene->GetGlobalSettings().GetAxisSystem();
			FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
			if (SceneAxisSystem != OurAxisSystem)
			{
				OurAxisSystem.ConvertScene(mScene);
			}

			// Convert Unit System to what is used in this example, if needed
			FbxSystemUnit SceneSystemUnit = mScene->GetGlobalSettings().GetSystemUnit();
			if (SceneSystemUnit.GetScaleFactor() != 1.0)
			{
				//The unit in this example is centimeter.
				FbxSystemUnit::cm.ConvertScene(mScene);
			}

			// Get the list of all the animation stack.
			mScene->FillAnimStackNameArray(mAnimStackNameArray);

			// Get the list of all the cameras in the scene.
			FillCameraArray(mScene, mCameraArray);

			// Convert mesh, NURBS and patch into triangle mesh
			FbxGeometryConverter lGeomConverter(mSdkManager);
			lGeomConverter.Triangulate(mScene, /*replace*/true);

			// Bake the scene for one frame
			LoadCacheRecursive(mScene, mCurrentAnimLayer, mFileName, mSupportVBO);

			// Convert any .PC2 point cache data into the .MC format for 
			// vertex cache deformer playback.
			PreparePointCacheData(mScene, mCache_Start, mCache_Stop);

			// Get the list of pose in the scene
			FillPoseArray(mScene, mPoseArray);

			addConsoleInfo("File %s \nClick on the right mouse button to enter menu.", mFileName);
			// Initialize the frame period.
			mFrameTime.SetTime(0, 0, 0, 1, 0, mScene->GetGlobalSettings().GetTimeMode());

			// Print the keyboard shortcuts.
			addConsoleInfo("Play/Pause Animation: Space Bar.");
			addConsoleInfo("Camera Rotate: Left Mouse Button.");
			addConsoleInfo("Camera Pan: Left Mouse Button + Middle Mouse Button.");
			addConsoleInfo("Camera Zoom: Middle Mouse Button.");

			lResult = true;
		}
		else
		{
			// Import failed, set the scene status flag accordingly.
			mStatus = UNLOADED;
			addConsoleInfo("Unable to import file %s nError reported: %s", mFileName, mImporter->GetStatus().GetErrorString());
		}

		// Destroy the importer to release the file.
		mImporter->Destroy();
		mImporter = NULL;
	}

	return lResult;
}