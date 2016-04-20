#pragma once
#include "C3DNode.h"

class C3DCamera : public C3DNode
{
public:
	//��ǰ�����е������
	static C3DCamera* m_pCurrCamera;
	C3DCamera();
	~C3DCamera();

public:
	//���õ�ǰ�����������
	void SetActive();
	void SetEyePosition(Vec3& vPos);
	void SetLookAtPosition(Vec3& vPos);
	void SetUpDir(Vec3& vDir);
	void SetLookAtTarget(C3DNode* pTarget);
public:
	virtual void Update();
	virtual void Render();

public:
	void UpdateViewMatrix();

private:
	Vec3 m_EyePosition;
	Vec3 m_LookAtPosition;
	Vec3 m_UpDir;

	C3DNode* m_pTarget;
};

