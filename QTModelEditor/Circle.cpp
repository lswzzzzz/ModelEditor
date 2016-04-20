#include "Circle.h"


CCircle::CCircle(std::string filename, float scale, float r, float n)
	:mFilename(filename), mscale(scale), mRadius(r), mcount(n)
{

}


CCircle::~CCircle()
{
}

void CCircle::initVertexData(float scale, float radius, float n)
{
	radius = radius * scale;
	float angdegSpan = 360.0f / n;
	vCount = 3 * n;
	vertieces = new stShapeVertices[vCount];
	int count = 0;
	for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdegSpan){
		double angrad = CC_DEGREES_TO_RADIANS(angdeg);
		double angradNext = CC_DEGREES_TO_RADIANS(angdeg + angdegSpan);
		vertieces[count].Position.x = 0;
		vertieces[count].Position.y = 0;
		vertieces[count].Position.x = 0;
		vertieces[count].TexUV.x = 0.5f;
		vertieces[count].TexUV.y = 0.5f;
		vertieces[count].Normal.x = 0;
		vertieces[count].Normal.y = 0;
		vertieces[count].Normal.z = 1;
		count++;
		vertieces[count].Position.x = (float)(-radius*sin(angrad));
		vertieces[count].Position.y = (float)(radius*cos(angrad));
		vertieces[count].Position.z = 0;
		vertieces[count].TexUV.x = (float)(0.5f - 0.5f*sin(angrad));
		vertieces[count].TexUV.y = (float)(0.5f - 0.5f*cos(angrad));
		vertieces[count].Normal.x = 0;
		vertieces[count].Normal.y = 0;
		vertieces[count].Normal.z = 1;
		count++;
		vertieces[count].Position.x = (float)(-radius*sin(angradNext));
		vertieces[count].Position.y = (float)(radius*cos(angradNext));
		vertieces[count].Position.z = 0;
		vertieces[count].TexUV.x = (float)(0.5f - 0.5f*sin(angradNext));
		vertieces[count].TexUV.y = (float)(0.5f - 0.5f*cos(angradNext));
		vertieces[count].Normal.x = 0;
		vertieces[count].Normal.y = 0;
		vertieces[count].Normal.z = 1;
		count++;
}