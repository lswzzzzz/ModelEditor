#include "C3DCamera.h"

C3DCamera* C3DCamera::m_pCurrCamera = NULL;

C3DCamera::C3DCamera()
{
	m_EyePosition = Vec3(0, 0, -1);
	m_LookAtPosition = Vec3(0, 0, 0);
	m_UpDir = Vec3(0, 1, 0);
	m_pTarget = NULL;
}


C3DCamera::~C3DCamera()
{
}

void C3DCamera::SetEyePosition(Vec3& vPos)
{
	m_EyePosition = vPos;
}

void C3DCamera::SetLookAtPosition(Vec3& vPos)
{
	m_LookAtPosition = vPos;
}

void C3DCamera::SetUpDir(Vec3& vDir)
{
	m_UpDir = vDir;
}

void C3DCamera::SetActive()
{
	C3DCamera::m_pCurrCamera = this;
}

void C3DCamera::UpdateViewMatrix()
{
	//取得当前节点在3D世界中的变换矩阵
	Mat4 tWorldMatrix = GetWorldMatrix();
	//对各向量进行变换
	Vec3 tRealEyePosition;
	tWorldMatrix.transformPoint(m_EyePosition, &tRealEyePosition);

	Vec3 tRealLookAtPosition;
	if (m_pTarget){
		m_pTarget->GetWorldMatrix().getTranslation(&tRealLookAtPosition);
	}
	else{
		tWorldMatrix.transformPoint(m_LookAtPosition, &tRealLookAtPosition);
	}
	
	//生成最终的观察矩阵
	Mat4 tViewMatrix;
	kmMat4LookAt(&tViewMatrix, &tRealEyePosition, &tRealLookAtPosition, &m_UpDir);
	//设置这个观察矩阵为OPENGL ES当前使用的观察矩阵
	kmGLMatrixMode(KM_GL_MODELVIEW);
	kmGLLoadIdentity();
	kmGLMultMatrix(&tViewMatrix);
}

void C3DCamera::SetLookAtTarget(C3DNode* pTarget)
{
	m_pTarget = pTarget;
}

void C3DCamera::Update()
{
	if (m_pFirstChild){
		m_pFirstChild->Update();
	}

	UpdateViewMatrix();

	if (m_pFirstBorther){
		m_pFirstBorther->Update();
	}
}

void C3DCamera::Render()
{
	C3DNode::Render();
}
