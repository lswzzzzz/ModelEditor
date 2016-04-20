#include "C3DSubMesh.h"
#include <stdio.h>
#include "Global.h"

C3DSubMesh::C3DSubMesh(void)
{
	
}


C3DSubMesh::~C3DSubMesh(void)
{

}

bool C3DSubMesh::LoadMeshFromFile(FILE* pFile)
{
	Release();
	
	if(pFile){
		stSubMeshHeader	tHeader;
		fread(&tHeader, sizeof(stSubMeshHeader), 1, pFile);
		SetName(tHeader.m_SubMeshName);
		//设置纹理
		std::string text = tHeader.m_Texture;
		std::string::size_type pos = text.find_last_of(".");
		text = text.substr(0, pos);
		text += ".png";
		SetTexture(text.c_str());
		m_VertexCount = tHeader.m_VertexCount;
		m_IndexCount = tHeader.m_IndexCount;
		m_PrimitiveType = tHeader.m_PrimitiveType;
		m_IndexFormat = tHeader.m_IndexFormat;

		//创建顶点与索引数组并读取数据
		if (0 == tHeader.m_WeightCount)
		{
			m_VertexArray = new stShapeVertices[m_VertexCount];
			fread(m_VertexArray, sizeof(stShapeVertices), m_VertexCount, pFile);
			m_IndiceArray = new GLushort[m_IndexCount];
			fread(m_IndiceArray, sizeof(GLushort), m_IndexCount, pFile);

		}
		else
		{
			m_SkinVertexArray = new stSkinVertices[m_VertexCount];
			memset(m_SkinVertexArray, 0, sizeof(stSkinVertices)*m_VertexCount);
			for (int v = 0; v < tHeader.m_VertexCount; v++)
			{
				stShapeVertices	tNewVertex;
				fread(&tNewVertex, sizeof(stShapeVertices), 1, pFile);
				m_SkinVertexArray[v].Position = tNewVertex.Position;
				m_SkinVertexArray[v].Normal = tNewVertex.Normal;
				m_SkinVertexArray[v].Color = Vec4(tNewVertex.Color.x, tNewVertex.Color.y, tNewVertex.Color.z, 1);
				m_SkinVertexArray[v].TexUV = tNewVertex.TexUV;
			}
			m_IndiceArray = new GLushort[m_IndexCount];
			fread(m_IndiceArray, sizeof(GLushort), m_IndexCount, pFile);
			for (int w = 0; w < tHeader.m_WeightCount; w++)
			{
				stVertex_BoneWeight	tBoneWeight;
				fread(&tBoneWeight, sizeof(stVertex_BoneWeight), 1, pFile);
				if (m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.x == 0.0)
				{
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Indices.x = tBoneWeight.mBoneIndex;
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.x = tBoneWeight.mWeight;
				}
				else if (m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.y == 0.0)
				{
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Indices.y = tBoneWeight.mBoneIndex;
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.y = tBoneWeight.mWeight;
				}
				else if (m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.z == 0.0)
				{
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Indices.z = tBoneWeight.mBoneIndex;
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.z = tBoneWeight.mWeight;
				}
				else
				{
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Indices.w = tBoneWeight.mBoneIndex;
					m_SkinVertexArray[tBoneWeight.mVertexIndex].Blend.w = tBoneWeight.mWeight;
				}

			}
		}

		//矩阵
		Mat4 tSubMatrix;
		fread(&tSubMatrix, sizeof(Mat4), 1, pFile);

		tSubMatrix.decompose(&m_Scale_Self, &m_Rotate_Self, &m_Translate_Self);
		m_Translate_Parent = Vec3(0, 0, 0);
		m_Scale_Parent = Vec3(1, 1, 1);
		m_Rotate_Parent.identity();
		/*
		//相对自身中心点的位置偏移
		fread(&m_Translate_Self,sizeof(Vec3),1,pFile);
		//自身自身中心点的缩放
		fread(&m_Scale_Self,sizeof(Vec3),1,pFile);
		//自身自身中心点的旋转
		fread(&m_Rotate_Self,sizeof(Quaternion),1,pFile);

		//相对父节点的位置偏移
		fread(&m_Translate_Parent,sizeof(Vec3),1,pFile);
		//自身自身中心点的缩放
		fread(&m_Scale_Parent,sizeof(Vec3),1,pFile);
		//相对父节点的旋转
		fread(&m_Rotate_Parent,sizeof(Quaternion),1,pFile);
		*/

		//创建VB与IB
		glGenBuffers(1, &m_VertexBuffer);
		glGenBuffers(1, &m_IndexBuffer);
		if (0 == tHeader.m_WeightCount)
		{
			//绑定数据到VB中。
			glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER_ARB,
				m_VertexCount * sizeof(stShapeVertices),
				m_VertexArray,
				GL_STATIC_DRAW);
		}
		else
		{
			//绑定数据到VB中。
			glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
			glBufferData(GL_ARRAY_BUFFER_ARB,
				m_VertexCount * sizeof(stSkinVertices),
				m_SkinVertexArray,
				GL_STATIC_DRAW);
		}
		//绑定数据到IB中。
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount*sizeof(GLushort), m_IndiceArray, GL_STATIC_DRAW);

		BuildShader();
		return true;
	}
	return false;
}

bool C3DSubMesh::WriteMeshToFile(FILE* pFile)
{
	if(pFile){
		stSubMeshHeader	tHeader;

		strcpy(tHeader.m_SubMeshName, GetName());
		strcpy(tHeader.m_Texture, m_TextureFileName.c_str());

		tHeader.m_VertexCount = m_VertexCount;
		tHeader.m_IndexCount = m_IndexCount;
		tHeader.m_WeightCount = 0;
		tHeader.m_PrimitiveType = m_PrimitiveType;
		tHeader.m_IndexFormat = m_IndexFormat;
		fwrite(&tHeader, sizeof(stSubMeshHeader), 1, pFile);

		fwrite(m_VertexArray, sizeof(stShapeVertices), m_VertexCount, pFile);
		fwrite(m_IndiceArray, sizeof(GLushort), m_IndexCount, pFile);

		//相对自身中心点的位置偏移
		fwrite(&m_Translate_Self, sizeof(Vec3), 1, pFile);
		//自身自身中心点的缩放
		fwrite(&m_Scale_Self, sizeof(Vec3), 1, pFile);
		//自身自身中心点的旋转
		fwrite(&m_Rotate_Self, sizeof(Quaternion), 1, pFile);
		/*
		//相对父节点的位置偏移
		fwrite(&m_Translate_Parent,sizeof(Vec3),1,pFile);
		//自身自身中心点的缩放
		fwrite(&m_Scale_Parent,sizeof(Vec3),1,pFile);
		//相对父节点的旋转
		fwrite(&m_Rotate_Parent,sizeof(Quaternion),1,pFile);
		*/

		return true;
	}
	return false;
}

bool C3DSubMesh::LoadMeshFromC3T(const char* c3t)
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	ssize_t size = 0;
	std::string path = FileUtils::getInstance()->fullPathForFilename(c3t);
	unsigned char* titlech = FileUtils::getInstance()->getFileData(path, "r", &size);
	std::string load_str = std::string((const char*)titlech, size);
	rapidjson::Document document;
	document.Parse<0>(load_str.c_str());
	if (document.HasParseError() || !document.IsObject()){
		return false;
	}
	rapidjson::Value& meshs = document["meshes"];
	rapidjson::Value& materials = document["materials"];
	rapidjson::Value& nodes = document["nodes"];
	rapidjson::Value& animations = document["animations"];
	rapidjson::SizeType index = 0;
	rapidjson::Value& mesh = meshs[index];
	rapidjson::Value& vertices = mesh["vertices"];
	rapidjson::Value& indices = mesh["parts"][index]["indices"];
	int structSize = 3 + 3 + 2 + 4 + 4;
	m_VertexCount = vertices.Capacity() / structSize;
	m_IndexCount = indices.Capacity();
	m_SkinVertexArray = new stSkinVertices[m_VertexCount];
	m_IndiceArray = new GLushort[m_IndexCount];
	memset(m_SkinVertexArray, 0, sizeof(stSkinVertices)*m_VertexCount);
	memset(m_IndiceArray, 0, sizeof(GLushort)*m_IndexCount);
	m_PrimitiveType = PT_TRIANGLES;
	m_IndexFormat = INDEX16;
	draw_type = SHAPE_C3T;
	for (int v = 0; v < m_VertexCount; v++){
		int index = 0;
		m_SkinVertexArray[v].Position.x = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Position.y = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Position.z = vertices[v*structSize + index++].GetDouble();

		m_SkinVertexArray[v].Normal.x = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Normal.y = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Normal.z = vertices[v*structSize + index++].GetDouble();

		m_SkinVertexArray[v].TexUV.x = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].TexUV.y = vertices[v*structSize + index++].GetDouble();

		m_SkinVertexArray[v].Blend.x = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Blend.y = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Blend.z = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Blend.w = vertices[v*structSize + index++].GetDouble();

		m_SkinVertexArray[v].Indices.x = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Indices.y = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Indices.z = vertices[v*structSize + index++].GetDouble();
		m_SkinVertexArray[v].Indices.w = vertices[v*structSize + index++].GetDouble();
	}

	for (int i = 0; i < m_IndexCount; i++){
		m_IndiceArray[i] = indices[i].GetInt();
	}

	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stSkinVertices), m_SkinVertexArray, GL_STATIC_DRAW);

	//绑定数据到IB中。
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount*sizeof(GLushort), m_IndiceArray, GL_STATIC_DRAW);

	std::string texture = materials[index]["textures"][index]["filename"].GetString();
	SetTexture(texture.c_str(), true);

	for (int i = 0; i < nodes.Capacity(); i++){
		RecursiveNode(nodes[i]);
	}

	m_BoneMatrixCount = translationVec.size();
	m_BoneMatrixArray = new Mat4[m_BoneMatrixCount];
	for (int i = 0; i < m_BoneMatrixCount; i++){
		m_BoneMatrixArray[i] = translationVec.at(i);
	}
	for (int i = 0; i < idVec.size(); i++){
		addConsoleInfo(idVec.at(i).c_str());
	}
	BuildShader();
	return true;
}

void C3DSubMesh::RecursiveNode(rapidjson::Value& node, bool isskeleton)
{
	if (isskeleton || node["skeleton"].GetBool()){
		Mat4 mat;
		if (node.HasMember("transform")){
			rapidjson::Value& transform = node["transform"];
			for (rapidjson::SizeType i = 0; i < transform.Capacity(); i++){
				mat.m[i] = transform[i].GetDouble();
			}
			translationVec.push_back(mat);
			idVec.push_back(node["id"].GetString());
		}
		isskeleton = true;
	}
	if (node.HasMember("children")){
		rapidjson::Value& children = node["children"];
		for (rapidjson::SizeType i = 0; i < children.Capacity(); i++){
			rapidjson::Value& nd = children[i];
			RecursiveNode(nd, isskeleton);
		}
	}
}
