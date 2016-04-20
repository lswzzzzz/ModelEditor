#pragma once

#include "C3DShape.h"
#include "SceneContext.h"

class FBX_Conv : public C3DShape
{
public:
	FBX_Conv();
	~FBX_Conv();
	virtual void Update();
	void LoadFile(const char* filename);
	void ConvData(const char* filename);
private:
	SceneContext* sceneContext;
};

