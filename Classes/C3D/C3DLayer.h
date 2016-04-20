#pragma once

#include "cocos2d.h"
#include "C3DShape.h"
#include "C3DParticleSystem.h"
#include "C3DCamera.h"
#include "C3DMesh.h"
#include "FBX_Conv.h"

USING_NS_CC;

class C3DLayer : public Layer
{
public:
	C3DLayer(void);
	~C3DLayer(void);

public:
	virtual bool init();
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags);
	CREATE_FUNC(C3DLayer);
protected:
	void addTouchEvent();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);
private:
	virtual void UpdateViewProjMatrix();
	virtual void Render3DWorld();

private:
	C3DMesh* m_mesh;
	C3DParticleSystem *m_pParticleSystem;
	C3DShape* shape;
	C3DCamera* m_pCamera;
	float m_fRotateYAngle;
	FBX_Conv* m_conv;

	float rotatex, rotatey;
	bool touched;
};

