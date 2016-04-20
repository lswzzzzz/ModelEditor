#pragma once
#include "C3DNode.h"
#include "C3DShape.h"

using namespace std;

class CCircle : public C3DNode
{
public:
	CCircle(std::string filename, float scale, float r, float n);
	~CCircle();
	void initVertexData(float scale, float radius, float count);
	void initShader();

private:
	std::string mFilename;
	float mscale;
	float mRadius;
	float mcount;
	stShapeVertices* vertieces;
	int vCount;
};

