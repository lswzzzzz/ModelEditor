#include "FBX_Conv.h"
#include "Exporter.h"

using namespace cocos2d;

FBX_Conv::FBX_Conv()
	:sceneContext(NULL)
{
}


FBX_Conv::~FBX_Conv()
{
}

void FBX_Conv::Update()
{
	if (sceneContext){
		sceneContext->OnDisplay();
		sceneContext->DisplayCallBack();
	}
}

void FBX_Conv::LoadFile(const char* filename)
{
	sceneContext = new SceneContext(filename, true);
	sceneContext->LoadFile();
}

void FBX_Conv::ConvData(const char* filename)
{
	Exporter* porter = new Exporter(filename);
	porter->LoadFile();
	std::string path = FileUtils::getInstance()->getSearchPaths().at(0);
	//porter->SaveMeshToFile((path+"test.mx").c_str());
	//porter->LoadMeshFromFile((path + "test.mx").c_str());
}