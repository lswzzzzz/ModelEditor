#include "Exporter.h"
#include "Common/Common.h"
#include "Global.h"
#include "SceneUtils.h"


std::vector<int> GetIndexList(FbxMesh* mesh);
std::vector<Vec3> GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList);
std::vector<Vec3> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList);
std::vector<Vec4> GetColorList(FbxMesh* mesh, const std::vector<int>& indexList);
std::vector<Vec2> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo);
void GetWeight(FbxMesh* mesh, const std::vector<int>& indexList, std::vector<ModelBoneWeight>& boneWeightList, std::vector<std::string>& boneNodeNameList, std::vector<Mat4>& invBaseposeMatrixList);
FbxAMatrix GetNodeGeometryTransform(FbxNode * pNode);

Exporter::Exporter(const char* filename)
	:mFileName(filename)
	,mFile(NULL)
{
	InitializeSdkObjects(mSdkManager, mScene);

	if (mSdkManager){
		int lFileFormat = -1;
		mImporter = FbxImporter::Create(mSdkManager, "");
		if (!mSdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(mFileName, lFileFormat))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			lFileFormat = mSdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
		}
		if (mImporter->Initialize(mFileName, lFileFormat, mSdkManager->GetIOSettings()) == true){
			addConsoleInfo("导入模型文件%s,请稍等!", mFileName);
		}
		else{
			addConsoleInfo("无法加载模型文件%s 错误数据返回为%s", mFileName, mImporter->GetStatus().GetErrorString());
		}
	}
	else{
		addConsoleInfo("无法创建FBX SDK Manager");
	}
}

Exporter::~Exporter()
{
}

bool Exporter::LoadFile()
{
	bool lResult = false;
	if (mImporter->Import(mScene) == true)
	{
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
		lGeomConverter.Triangulate(mScene, true);

		// Bake the scene for one frame
		LoadCacheRecursive(mScene, mCurrentAnimLayer, mFileName, true);

		// Convert any .PC2 point cache data into the .MC format for 
		// vertex cache deformer playback.
		PreparePointCacheData(mScene, mCache_Start, mCache_Stop);

		// Get the list of pose in the scene
		FillPoseArray(mScene, mPoseArray);

		// Initialize the frame period.
		mFrameTime.SetTime(0, 0, 0, 1, 0, mScene->GetGlobalSettings().GetTimeMode());

		mImporter->Destroy();
		mImporter = NULL;

		lResult = true;
	}

	return lResult;
}

void Exporter::RecursiveRoot()
{
	FbxNode* root = mScene->GetRootNode();
	string str = FileUtils::getInstance()->getSearchPaths().at(0) + "save.wb";
	mFile = fopen(str.c_str(), "wb");
	for (int i = 0; i < root->GetChildCount(); i++){
		RecursiveNode(root->GetChild(i));
	}
	fclose(mFile);
}

void Exporter::RecursiveNode(FbxNode* node)
{
	if (node->GetNodeAttribute()){
		GetCurNode(node);
	}
	const int childCount = node->GetChildCount();
	for (int i = 0; i < childCount; i++){
		RecursiveNode(node->GetChild(i));
	}
}

void Exporter::GetCurNode(FbxNode* node)
{
	FbxNodeAttribute* lNodeAttribute = node->GetNodeAttribute();

	if (lNodeAttribute)
	{
		// All lights has been processed before the whole scene because they influence every geometry.
		switch (lNodeAttribute->GetAttributeType()){
		case FbxNodeAttribute::eMarker:
			//addConsoleInfo("eMarker");
			
			break;
		case FbxNodeAttribute::eSkeleton:
			
			//addConsoleInfo("eSkeleton");
			break;
		case FbxNodeAttribute::eMesh:
			ReadMesh(node->GetMesh());
			//addConsoleInfo("eMesh");
			break;
		case FbxNodeAttribute::eCamera:
			
			//addConsoleInfo("eCamera");
		case FbxNodeAttribute::eLight:
			
			//addConsoleInfo("eLight");
			break;
		case FbxNodeAttribute::eNull:
		
			//addConsoleInfo("eNull");
			break;
		case FbxNodeAttribute::eBoundary:
			//addConsoleInfo("eBoundary");break;
		case FbxNodeAttribute::eOpticalMarker:
			//addConsoleInfo("eOpticalMarker");break;
		case FbxNodeAttribute::eOpticalReference:
			//addConsoleInfo("eOpticalReference");break;
		case FbxNodeAttribute::eCameraSwitcher:
			//addConsoleInfo("eCameraSwitcher");break;
		case FbxNodeAttribute::ePatch:
			//addConsoleInfo("ePatch");break;
		case FbxNodeAttribute::eNurbs:
			//addConsoleInfo("eNurbs");break;
		case FbxNodeAttribute::eNurbsSurface:
			//addConsoleInfo("eNurbsSurface");break;
		case FbxNodeAttribute::eNurbsCurve:
			//addConsoleInfo("eNurbsCurve");break;
		case FbxNodeAttribute::eTrimNurbsSurface:
			//addConsoleInfo("eTrimNurbsSurface");break;
		case FbxNodeAttribute::eCameraStereo:
			//addConsoleInfo("eCameraStereo");break;
		case FbxNodeAttribute::eCachedEffect:
			//addConsoleInfo("eCachedEffect");break;
		case FbxNodeAttribute::eLine:
			//addConsoleInfo("eLine");break;
		case FbxNodeAttribute::eLODGroup:
			//addConsoleInfo("eLODGroup");break;
		case FbxNodeAttribute::eShape:
			//addConsoleInfo("eShape");break;
		case FbxNodeAttribute::eSubDiv:
			//addConsoleInfo("eSubDiv");break;
		case FbxNodeAttribute::eUnknown:
			//addConsoleInfo("eUnknown");break;
			break;
		}
	}
	else
	{
		// Draw a Null for nodes without attribute.
		
	}
}

void Exporter::ReadMesh(FbxMesh* mesh)
{
	
}

void Exporter::SaveMeshToFile(const char* file)
{
	auto meshCount = this->mScene->GetMemberCount<FbxMesh>();
	for (int i = 0; i < meshCount; i++){
		auto fbxMesh = mScene->GetMember<FbxMesh>(i);
		auto modelMesh = ParseMesh(fbxMesh);
		m_meshList.push_back(modelMesh);
	}
	mFile = fopen(file, "wb");

	fwrite(&meshCount, sizeof(meshCount), 1, mFile);

	for (int i = 0; i < meshCount; i++){
		auto &modelMesh = m_meshList[i];
		int nameSize = modelMesh.nodeName.size();
		fwrite(&nameSize, sizeof(nameSize), 1, mFile);
		fwrite(modelMesh.nodeName.c_str(), nameSize, 1, mFile);
		int materialNameSize = modelMesh.materialName.size();
		fwrite(&materialNameSize, sizeof(materialNameSize), 1, mFile);
		fwrite(modelMesh.materialName.c_str(), materialNameSize, 1, mFile);
		int listsize = modelMesh.vertexList.size();
		fwrite(&listsize, sizeof(listsize), 1, mFile);
		for (int k = 0; k < modelMesh.vertexList.size(); k++){
			auto skin = modelMesh.vertexList.at(k);
			fwrite(&skin, sizeof(skin), 1, mFile);
		}
		fwrite(&modelMesh.invMeshBaseposeMatrix, sizeof(modelMesh.invMeshBaseposeMatrix), 1, mFile);
		listsize = modelMesh.boneNodeNameList.size();
		fwrite(&listsize, sizeof(listsize), 1, mFile);
		for (int k = 0; k < modelMesh.boneNodeNameList.size(); k++){
			auto str = modelMesh.boneNodeNameList.at(k);
			auto size = str.size();
			fwrite(&size, sizeof(size), 1, mFile);
			fwrite(str.c_str(), size, 1, mFile);
		}
		listsize = modelMesh.invBoneBaseposeMatrixList.size();
		fwrite(&listsize, sizeof(listsize), 1, mFile);
		for (int k = 0; k < listsize; k++){
			auto mat = modelMesh.invBoneBaseposeMatrixList.at(k);
			fwrite(&mat, sizeof(mat), 1, mFile);
		}
	}
	fclose(mFile);
}

void Exporter::LoadMeshFromFile(const char*file)
{
	int meshCount;
	mFile = fopen(file, "rb");
	m_meshList.clear();
	fread(&meshCount, sizeof(meshCount), 1, mFile);
	for (int i = 0; i < meshCount; i++){
		ModelMesh modelMesh;
		int size;
		fread(&size, sizeof(size), 1, mFile);
		char str[256];
		memset(str, 0, 256);
		fread(str, size, 1, mFile);
		modelMesh.nodeName = str;
		memset(str, 0, 256);
		fread(&size, sizeof(size), 1, mFile);
		fread(str, size, 1, mFile);
		modelMesh.materialName = str;
		fread(&size, sizeof(size), 1, mFile);
		for (int k = 0; k < size; k++){
			stSkinVertices skin;
			fread(&skin, sizeof(skin), 1, mFile);
			modelMesh.vertexList.push_back(skin);
		}
		Mat4 mat4;
		fread(&mat4, sizeof(mat4), 1, mFile);
		modelMesh.invMeshBaseposeMatrix = mat4;
		fread(&size, sizeof(size), 1, mFile);
		for (int k = 0; k < size; k++){
			int sz;
			memset(str, 0, 256);
			fread(&sz, sizeof(sz), 1, mFile);
			fread(str, sz, 1, mFile);
			modelMesh.boneNodeNameList.push_back(str);
		}
		fread(&size, sizeof(size), 1, mFile);
		for (int k = 0; k < size; k++){
			Mat4 mat;
			fread(&mat, sizeof(mat), 1, mFile);
			modelMesh.invBoneBaseposeMatrixList.push_back(mat);
		}
		m_meshList.push_back(modelMesh);
	}
	fclose(mFile);
}

ModelMesh Exporter::ParseMesh(FbxMesh* mesh)
{
	assert(mesh != nullptr);
	auto node = mesh->GetNode();
	ModelMesh modelMesh;
	if (node->GetMaterialCount() < 1){
		return modelMesh;
	}
	modelMesh.nodeName = node->GetName();
	modelMesh.materialName = node->GetMaterial(0)->GetName();

	auto baseposeMatrix = node->EvaluateGlobalTransform();//.Inverse();
	auto geometryMatrix = GetNodeGeometryTransform(node);
	baseposeMatrix = geometryMatrix*baseposeMatrix;
	auto baseposeMatrixPtr = (double*)baseposeMatrix;
	for (int i = 0; i < 16; i++) {
		modelMesh.invMeshBaseposeMatrix.m[i] = (double)baseposeMatrixPtr[i];
	}

	auto indexList = GetIndexList(mesh);

	auto positionList = GetPositionList(mesh, indexList);
	auto normalList = GetNormalList(mesh, indexList);
	auto uv0List = GetUVList(mesh, indexList, 0);

	std::vector<ModelBoneWeight> boneWeightList;
	GetWeight(mesh, indexList, boneWeightList, modelMesh.boneNodeNameList, modelMesh.invBoneBaseposeMatrixList);

	assert(indexList.size() == positionList.size());
	assert(indexList.size() == normalList.size());
	assert(indexList.size() == uv0List.size());
	assert(indexList.size() == boneWeightList.size() || (boneWeightList.size() == 0));

	std::vector<stSkinVertices> modelVertexList;
	modelVertexList.reserve(indexList.size());

	for (size_t i = 0; i < indexList.size(); i++) {
		stSkinVertices vertex;
		vertex.Position.x = positionList[i].x;
		vertex.Position.y = positionList[i].y;
		vertex.Position.z = positionList[i].z;

		vertex.Normal.x = normalList[i].x;
		vertex.Normal.y = normalList[i].y;
		vertex.Normal.z = normalList[i].z;

		if (uv0List.size() > 0) {
			vertex.TexUV.x = uv0List[i].x;
			vertex.TexUV.y = uv0List[i].y;
		}
		else {
			vertex.TexUV.x = 0;
			vertex.TexUV.y = 0;
		}

		if (boneWeightList.size() > 0) {
			vertex.Indices.x = boneWeightList[i].boneIndex[0];
			vertex.Blend.x = boneWeightList[i].boneWeight[0];
			vertex.Indices.y = boneWeightList[i].boneIndex[1];
			vertex.Blend.y = boneWeightList[i].boneWeight[1];
			vertex.Indices.z = boneWeightList[i].boneIndex[2];
			vertex.Blend.z = boneWeightList[i].boneWeight[2];
			vertex.Indices.w = boneWeightList[i].boneIndex[3];
			vertex.Blend.w = boneWeightList[i].boneWeight[3];
		}
		else {
			vertex.Indices.x = boneWeightList[i].boneIndex[0];
			vertex.Blend.x = boneWeightList[i].boneWeight[0];
			vertex.Indices.y = boneWeightList[i].boneIndex[1];
			vertex.Blend.y = boneWeightList[i].boneWeight[1];
			vertex.Indices.z = boneWeightList[i].boneIndex[2];
			vertex.Blend.z = boneWeightList[i].boneWeight[2];
			vertex.Indices.w = boneWeightList[i].boneIndex[3];
			vertex.Blend.w = boneWeightList[i].boneWeight[3];
		}

		modelVertexList.push_back(vertex);
	}

	// 去除重复顶点
	/*auto & modelVertexListOpt = modelMesh.vertexList;
	modelVertexListOpt.reserve(modelVertexList.size());

	auto & modelIndexList = modelMesh.indexList;
	modelIndexList.reserve(indexList.size());

	for (int i = 0; i < modelVertexList.size(); i++) {
		auto vertex = modelVertexList.at(i);
		std::vector<stSkinVertices>::iterator it = modelVertexListOpt.begin();
		for (; it != modelVertexListOpt.end(); it++){
			if (it->Blend == vertex.Blend && it->Color == vertex.Color && it->Indices == vertex.Indices
				&& it->Normal == vertex.Normal && it->Position == vertex.Position && it->TexUV == vertex.TexUV){
				break;
			}
		}
		if (it == modelVertexListOpt.end()) {
			modelIndexList.push_back(modelVertexListOpt.size());
			modelVertexListOpt.push_back(vertex);
		}
		else {
			auto index = std::distance(modelVertexListOpt.begin(), it);
			modelIndexList.push_back(index);
		}
	}*/

	//addConsoleInfo("Index: %lu -> %lu\n", indexList.size(), modelIndexList.size());
	//addConsoleInfo("Opt: %lu -> %lu\n", modelVertexList.size(), modelVertexListOpt.size());

	return modelMesh;
}

FbxAMatrix GetNodeGeometryTransform(FbxNode * pNode)
{
	FbxAMatrix mat;
	mat.SetIdentity();

	if (pNode->GetNodeAttribute()) {
		auto t = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		auto r = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		auto s = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		mat.SetT(t);
		mat.SetR(r);
		mat.SetS(s);
	}

	return mat;
}

std::vector<int> GetIndexList(FbxMesh* mesh)
{
	// あらかじめ三角形化してある
	auto polygonCount = mesh->GetPolygonCount();

	std::vector<int> indexList;
	indexList.reserve(polygonCount * 3);

	for (int i = 0; i < polygonCount; ++i)
	{
		indexList.push_back(mesh->GetPolygonVertex(i, 0));
		indexList.push_back(mesh->GetPolygonVertex(i, 1));
		indexList.push_back(mesh->GetPolygonVertex(i, 2));
	}

	return indexList;
}

std::vector<Vec3> GetPositionList(FbxMesh* mesh, const std::vector<int>& indexList)
{
	// コントロールポイントがいわゆる位置座標
	std::vector<Vec3> positionList;
	positionList.reserve(indexList.size());

	for (auto index : indexList)
	{
		auto controlPoint = mesh->GetControlPointAt(index);

		positionList.push_back(Vec3(controlPoint[0], controlPoint[1], controlPoint[2]));

		// wは0.0のみ対応
		assert(controlPoint[3] == 0.0);
	}

	return positionList;
}

std::vector<Vec3> GetNormalList(FbxMesh* mesh, const std::vector<int>& indexList)
{
	auto elementCount = mesh->GetElementNormalCount();

	// 法線要素が1のみ対応
	assert(elementCount == 1);

	auto element = mesh->GetElementNormal();
	auto mappingMode = element->GetMappingMode();
	auto referenceMode = element->GetReferenceMode();
	const auto& indexArray = element->GetIndexArray();
	const auto& directArray = element->GetDirectArray();

	// eDirctかeIndexDirectのみ対応
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	std::vector<Vec3> normalList;
	normalList.reserve(indexList.size());

	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// コントロールポイントでマッピング
		for (auto index : indexList)
		{
			auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
				? index
				: indexArray.GetAt(index);
			auto normal = directArray.GetAt(normalIndex);
			normalList.push_back(Vec3(normal[0], normal[1], normal[2]));

			// wは1.0のみ対応
			assert(normal[3] == 1.0);
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		// ポリゴンバーテックス（インデックス）でマッピング
		auto indexByPolygonVertex = 0;
		auto polygonCount = mesh->GetPolygonCount();
		for (int i = 0; i < polygonCount; ++i)
		{
			auto polygonSize = mesh->GetPolygonSize(i);

			for (int j = 0; j < polygonSize; ++j)
			{
				auto normalIndex = (referenceMode == FbxGeometryElement::eDirect)
					? indexByPolygonVertex
					: indexArray.GetAt(indexByPolygonVertex);
				auto normal = directArray.GetAt(normalIndex);

				normalList.push_back(Vec3(normal[0], normal[1], normal[2]));

				// wは1.0のみ対応
				//assert(normal[3] == 1.0);

				++indexByPolygonVertex;
			}

		}
	}
	else
	{
		assert(false);
	}

	return normalList;
}

std::vector<Vec4> GetColorList(FbxMesh* mesh, const std::vector<int>& indexList)
{
	std::vector<Vec4> colorList;
	colorList.reserve(indexList.size());
	auto colorCount = mesh->GetElementVertexColorCount();

	auto element = mesh->GetElementVertexColor();
	auto mappingMode = element->GetMappingMode();
	auto referenceMode = element->GetReferenceMode();
	const auto& indexArray = element->GetIndexArray();
	const auto& directArray = element->GetDirectArray();

	// eDirctかeIndexDirectのみ対応
	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		// コントロールポイントでマッピング
		for (auto index : indexList)
		{
			auto colorIndex = (referenceMode == FbxGeometryElement::eDirect)
				? index
				: indexArray.GetAt(index);
			auto color = directArray.GetAt(colorIndex);
			colorList.push_back(Vec4(color[0], color[1], color[2], color[3]));
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		
		auto indexByPolygonVertex = 0;
		auto polygonCount = mesh->GetPolygonCount();
		for (int i = 0; i < polygonCount; ++i)
		{
			auto polygonSize = mesh->GetPolygonSize(i);

			for (int j = 0; j < polygonSize; ++j)
			{
				auto colorIndex = (referenceMode == FbxGeometryElement::eDirect)
					? indexByPolygonVertex
					: indexArray.GetAt(indexByPolygonVertex);
				auto color = directArray.GetAt(colorIndex);

				colorList.push_back(Vec4(color[0], color[1], color[2], color[3]));

				++indexByPolygonVertex;
			}

		}
	}
	else
	{
		assert(false);
	}

	return colorList;

}

std::vector<Vec2> GetUVList(FbxMesh* mesh, const std::vector<int>& indexList, int uvNo)
{
	std::vector<Vec2> uvList;

	auto elementCount = mesh->GetElementUVCount();
	if (uvNo + 1 > elementCount)
	{
		return uvList;
	}

	auto element = mesh->GetElementUV(uvNo);
	auto mappingMode = element->GetMappingMode();
	auto referenceMode = element->GetReferenceMode();
	const auto& indexArray = element->GetIndexArray();
	const auto& directArray = element->GetDirectArray();

	assert((referenceMode == FbxGeometryElement::eDirect) || (referenceMode == FbxGeometryElement::eIndexToDirect));

	uvList.reserve(indexList.size());

	if (mappingMode == FbxGeometryElement::eByControlPoint)
	{
		for (auto index : indexList)
		{
			auto uvIndex = (referenceMode == FbxGeometryElement::eDirect)
				? index
				: indexArray.GetAt(index);
			auto uv = directArray.GetAt(uvIndex);
			uvList.push_back(Vec2(uv[0], uv[1]));
		}
	}
	else if (mappingMode == FbxGeometryElement::eByPolygonVertex)
	{
		auto indexByPolygonVertex = 0;
		auto polygonCount = mesh->GetPolygonCount();
		for (int i = 0; i < polygonCount; ++i)
		{
			auto polygonSize = mesh->GetPolygonSize(i);

			for (int j = 0; j < polygonSize; ++j)
			{
				auto uvIndex = (referenceMode == FbxGeometryElement::eDirect)
					? indexByPolygonVertex
					: indexArray.GetAt(indexByPolygonVertex);
				auto uv = directArray.GetAt(uvIndex);

				uvList.push_back(Vec2(uv[0], uv[1]));

				++indexByPolygonVertex;
			}

		}
	}
	else
	{
		assert(false);
	}

	return uvList;
}

void GetWeight(FbxMesh* mesh, const std::vector<int>& indexList, std::vector<ModelBoneWeight>& boneWeightList, std::vector<std::string>& boneNodeNameList, std::vector<Mat4>& invBaseposeMatrixList)
{
	auto skinCount = mesh->GetDeformerCount(FbxDeformer::eSkin);
	if (skinCount == 0)
	{
		return;
	}

	// スキンが1しか対応しない
	assert(skinCount <= 1);

	auto controlPointsCount = mesh->GetControlPointsCount();
	std::vector<std::vector<std::pair<int, float>>> tmpBoneWeightList(controlPointsCount);

	auto skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
	auto clusterCount = skin->GetClusterCount();

	for (int i = 0; i < clusterCount; ++i)
	{
		auto cluster = skin->GetCluster(i);

		// eNormalizeしか対応しない
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);

		boneNodeNameList.push_back(cluster->GetLink()->GetName());

		auto indexCount = cluster->GetControlPointIndicesCount();
		auto indices = cluster->GetControlPointIndices();
		auto weights = cluster->GetControlPointWeights();

		for (int j = 0; j < indexCount; ++j)
		{
			auto controlPointIndex = indices[j];
			tmpBoneWeightList[controlPointIndex].push_back({ i, weights[j] });
		}

		// ペースポーズの逆行列を作成しておく
		Mat4 invBaseposeMatrix;

		auto baseposeMatrix = cluster->GetLink()->EvaluateGlobalTransform().Inverse();
		auto baseposeMatrixPtr = (double*)baseposeMatrix;
		for (int j = 0; j < 16; ++j)
		{
			invBaseposeMatrix.m[j] = (float)baseposeMatrixPtr[j];
		}

		invBaseposeMatrixList.push_back(invBaseposeMatrix);
	}

	// コントロールポイントに対応したウェイトを作成
	std::vector<ModelBoneWeight> boneWeightListControlPoints;
	for (auto& tmpBoneWeight : tmpBoneWeightList)
	{
		// ウェイトの大きさでソート
		std::sort(tmpBoneWeight.begin(), tmpBoneWeight.end(),
			[](const std::pair<int, float>& weightA, const std::pair<int, float>& weightB){ return weightA.second > weightB.second; }
			//[](const TmpWeight& weightA, const TmpWeight& weightB){ return weightA.second < weightB.second; }
		);

		// 1頂点に4つより多くウェイトが割り振られているなら影響が少ないものは無視する
		while (tmpBoneWeight.size() > 4)
		{
			tmpBoneWeight.pop_back();
		}

		// 4つに満たない場合はダミーを挿入
		while (tmpBoneWeight.size() < 4)
		{
			tmpBoneWeight.push_back({ 0, 0.0f });
		}

		ModelBoneWeight weight;
		float total = 0.0f;
		for (int i = 0; i < 4; ++i)
		{
			weight.boneIndex[i] = tmpBoneWeight[i].first;
			weight.boneWeight[i] = tmpBoneWeight[i].second;

			total += tmpBoneWeight[i].second;
		}

		// ウェイトの正規化
		for (int i = 0; i < 4; ++i)
		{
			weight.boneWeight[i] /= total;
		}

		boneWeightListControlPoints.push_back(weight);
	}

	// インデックスで展開
	for (auto index : indexList)
	{
		boneWeightList.push_back(boneWeightListControlPoints[index]);
	}
}