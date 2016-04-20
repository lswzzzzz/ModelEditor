#include "C3DShape.h"
#include "comman/texture.h"
#include "comman/objloader.h"
#include "comman/vboindexer.h"
#include "Global.h"


C3DShape::C3DShape(void):m_VertexBuffer(0)
	,m_IndexBuffer(0)
	, m_PrimitiveType(PT_TRIANGLES)
	,m_VertexArray(0)
	,m_IndiceArray(0)
	,m_VertexCount(0)
	,m_IndexCount(0)
	, m_SkinVertexArray(0)
	, m_BoneMatrixCount(0)
	,m_ShaderProgram(NULL)
	, m_IndexFormat(INDEX16)
	,m_Texture(NULL)
	, is_DrawColor(false)
	, is_Draw_Texture(false)
	, draw_type(SHAPE_USERDEFINE)
{
	m_BlendFunc = BlendFunc::ALPHA_NON_PREMULTIPLIED;
}

void C3DShape::CreatePoints()
{
	Release();
	draw_type = SHAPE_POINTS;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	auto size = Director::getInstance()->getVisibleSize();
	glGenBuffers(1, &m_VertexBuffer);
	//创建数组并填充数据
	m_VertexArray = new stShapeVertices[100];
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			float x, y;
			x = sin(i * 18 - 90);
			y = cos(j * 18 - 180);
			//m_VertexArray[i*10+j].Position = Vec3((i*size.width/20+50)/(size.width), (j*size.height/20+50)/(size.width), 0);
			m_VertexArray[i * 10 + j].Position = Vec3(x, y, 0);
		}
	}
	m_VertexCount = 100;
	m_IndexCount = 0;
	m_PrimitiveType = PT_POINTS;

	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices),
		m_VertexArray, GL_STATIC_DRAW);
	BuildShader();
}

void C3DShape::CreateLines()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	draw_type = SHAPE_LINE;
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	//创建顶点数组
	m_VertexArray = new stShapeVertices[23];
	m_VertexArray[0].Position = kmVec3(-1, -1, 0);
	m_VertexArray[1].Position = kmVec3(-1, 1, 0);
	m_VertexArray[2].Position = kmVec3(0.5, 1, 0);
	m_VertexArray[3].Position = kmVec3(0.7, 1, 0);
	m_VertexArray[4].Position = kmVec3(1, 1, 0);
	m_VertexArray[5].Position = kmVec3(1, -1, 0);
	m_VertexArray[6].Position = kmVec3(0.8, -0.7, 0);
	m_VertexArray[7].Position = kmVec3(-0.5, -0.7, 0);
	m_VertexArray[8].Position = kmVec3(-0.5, 0, 0);
	m_VertexArray[9].Position = kmVec3(-0.6, 0.7, 0);
	m_VertexArray[10].Position = kmVec3(0, 0.7, 0);
	m_VertexArray[11].Position = kmVec3(0, 0, 0);
	m_VertexArray[12].Position = kmVec3(0.8, -0.5, 0);
	m_VertexArray[13].Position = kmVec3(0.8, 0, 0);
	m_VertexArray[14].Position = kmVec3(0.6, 0, 0);
	m_VertexArray[15].Position = kmVec3(0.6, 0.7, 0);
	m_VertexArray[16].Position = kmVec3(-0.8, -1, 0);
	m_VertexArray[17].Position = kmVec3(-0.6, 0.4, 0);
	m_VertexArray[18].Position = kmVec3(-0.2, 0.4, 0);
	m_VertexArray[19].Position = kmVec3(-0.2, -0.4, 0);
	m_VertexArray[20].Position = kmVec3(-0.6, 0.2, 0);
	m_VertexArray[21].Position = kmVec3(-0.4, 0.2, 0);
	m_VertexArray[22].Position = kmVec3(-0.4, -0.4, 0);
	//创建索引数组
	float indices[32] = {
		0, 1, 1, 2, 3, 4, 4, 5, 5, 16,
		6, 7, 7, 8, 9, 10, 10, 11,
		12, 13, 13, 14, 14, 15,
		17, 18, 18, 19, 20, 21, 21, 22
	};
	m_IndiceArray = new GLushort[24];
	for (int i = 0; i < 32; i++){
		m_IndiceArray[i] = indices[i];
	}

	m_VertexCount = 23;
	m_IndexCount = 32;
	m_PrimitiveType = PT_LINES;
	m_IndexFormat = INDEX16;
	//绑定数据到VB中
	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices),
		m_VertexArray, GL_STATIC_DRAW);

	//绑定下标到IB
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount * sizeof(GLushort),
		m_IndiceArray, GL_STATIC_DRAW);
	BuildShader();
}

void C3DShape::CreateTriangles()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	draw_type = SHAPE_TRIANGLES;
	is_DrawColor = true;
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	m_VertexArray = new stShapeVertices[4];
	m_VertexArray[0].Position = Vec3(0, 1, 0);
	m_VertexArray[1].Position = Vec3(-1, -1, 1);
	m_VertexArray[2].Position = Vec3(1, -1, 1);
	m_VertexArray[3].Position = Vec3(0, -1, -1);
	m_VertexArray[0].TexUV = Vec2(0, 1);
	m_VertexArray[1].TexUV = Vec2(1, 1);
	m_VertexArray[2].TexUV = Vec2(0, 0);
	m_VertexArray[3].TexUV = Vec2(1, 0);
	m_VertexArray[0].Color = Vec3(1, 0, 0);
	m_VertexArray[1].Color = Vec3(0, 1, 0);
	m_VertexArray[2].Color = Vec3(0, 0, 1);
	m_VertexArray[3].Color = Vec3(1, 1, 0);
	float indices[4 * 3] = { 0, 1, 2,
		0, 2, 3,
		0, 3, 1,
		1, 2, 3
	};
	m_IndiceArray = new GLushort[4 * 3];
	for (int i = 0; i < 4 * 3; i++){
		m_IndiceArray[i] = indices[i];
	}
	m_VertexCount = 4;
	m_IndexCount = 4 * 3;
	m_PrimitiveType = PT_TRIANGLES;
	m_IndexFormat = INDEX16;

	//绑定数据到VB
	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices),
		m_VertexArray, GL_STATIC_DRAW);

	//绑定数据到IB中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount * sizeof(GLushort),
		m_IndiceArray, GL_STATIC_DRAW);
	BuildShader();
}

void C3DShape::CreateRectangle()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	draw_type = SHAPE_RECTANGLE;
	is_DrawColor = true;
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	///建顶点数组
	m_VertexArray = new stShapeVertices[8];
	m_VertexArray[0].Position = Vec3(-1, -1, 0);
	m_VertexArray[1].Position = Vec3(-1, 1, 0);
	m_VertexArray[2].Position = Vec3(1, 1, 0);
	m_VertexArray[3].Position = Vec3(1, -1, 0);
	m_VertexArray[4].Position = Vec3(-1, -1, 2);
	m_VertexArray[5].Position = Vec3(-1, 1, 2);
	m_VertexArray[6].Position = Vec3(1, 1, 2);
	m_VertexArray[7].Position = Vec3(1, -1, 2);
	m_VertexArray[0].TexUV = Vec2(0, 1);
	m_VertexArray[1].TexUV = Vec2(1, 1);
	m_VertexArray[2].TexUV = Vec2(1, 0);
	m_VertexArray[3].TexUV = Vec2(0, 0);
	m_VertexArray[4].TexUV = Vec2(0, 0);
	m_VertexArray[5].TexUV = Vec2(1, 0);
	m_VertexArray[6].TexUV = Vec2(1, 1);
	m_VertexArray[7].TexUV = Vec2(0, 1);
	m_VertexArray[0].Color = Vec3(1, 0, 0);
	m_VertexArray[1].Color = Vec3(0, 1, 0);
	m_VertexArray[2].Color = Vec3(0, 0, 1);
	m_VertexArray[3].Color = Vec3(1, 1, 0);
	m_VertexArray[4].Color = Vec3(0, 1, 1);
	m_VertexArray[5].Color = Vec3(1, 0, 1);
	m_VertexArray[6].Color = Vec3(0, 0, 1);
	m_VertexArray[7].Color = Vec3(1, 0, 0);
	//创建索引数组
	float indices[12 * 3] = { 0, 1, 2,
		0, 2, 3,
		2, 6, 7,
		2, 3, 7,
		5, 6, 7,
		5, 4, 7,
		5, 1, 0,
		0, 4, 5,
		0, 3, 7,
		0, 4, 7,
		1, 2, 6,
		1, 5, 6
	};
	m_IndiceArray = new GLushort[12 * 3];
	for (int i = 0; i < 12 * 3; i++){
		m_IndiceArray[i] = indices[i];
	}
	m_VertexCount = 8;
	m_IndexCount = 12 * 3;
	m_PrimitiveType = PT_TRIANGLES;
	m_IndexFormat = INDEX16;

	//绑定数据到VB
	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices),
		m_VertexArray, GL_STATIC_DRAW);

	//绑定数据到IB中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount * sizeof(GLushort),
		m_IndiceArray, GL_STATIC_DRAW);
	BuildShader();
}

C3DShape::~C3DShape(void)
{
	Release();
	CC_SAFE_DELETE(m_ShaderProgram);
	CC_SAFE_RELEASE(m_Texture);
}

void C3DShape::CreateSphere()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	draw_type = SHAPE_SPHERE;
	int nRings = 40;
	int nSegments = 40;
	//每个环的角度
	float deltaRing = M_PI/(nRings);
	float deltaSegment = 2.0f*M_PI/nSegments;
	m_VertexCount = (nRings + 1) * (nSegments + 1);
	m_VertexArray = new stShapeVertices[m_VertexCount];
	stShapeVertices* pCurrVertex = m_VertexArray;
	m_IndexCount = nRings * (nSegments + 1) * 2;
	m_IndiceArray = new GLushort[m_IndexCount];
	GLushort* pCurrIndex = m_IndiceArray;
	GLushort vIndex = 0;
	m_PrimitiveType = PT_TRIANGLE_STRIP;
	m_IndexFormat = INDEX16;

	//创建球的算法
	for(int j=0; j<nRings+1; j++){
		float radius = sinf(j*deltaRing);
		float y0 = cosf(j*deltaRing);

		for(int i=0; i<nSegments+1; i++){
			float x0 = radius * sinf(i * deltaSegment);
			float z0 = radius * cosf(i * deltaSegment);
			pCurrVertex->Position.x = x0;
			pCurrVertex->Position.y = y0;
			pCurrVertex->Position.z = z0;
			pCurrVertex->TexUV.x = float(i)/nSegments;
			pCurrVertex->TexUV.y = float(j)/nRings;
			//log("%f %f", pCurrVertex->TexUV.x, pCurrVertex->TexUV.y);
			pCurrVertex++;

			//除了第一点和最后一点只有一次，其他点都有两次索引
			if(j != nRings){
				*pCurrIndex = vIndex;
				pCurrIndex++;

				*pCurrIndex = vIndex + (GLushort)(nSegments + 1);
				pCurrIndex++;
				vIndex++;
			}
		}
	}

	//绑定数据到VB中
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(stShapeVertices),
					m_VertexArray, GL_STATIC_DRAW);

	//IB
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(GLushort),
				m_IndiceArray, GL_STATIC_DRAW);
	BuildShader();
}

void C3DShape::CreateObj(const char* obj)
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	draw_type = SHAPE_3DMAX_OBJ;
	std::vector<Vec3> vertices;
	std::vector<Vec2> uvs;
	std::vector<Vec3> normals;
	std::string path = FileUtils::getInstance()->fullPathForFilename(obj);
	std::vector<unsigned short> indices;
	std::vector<Vec3> indexed_vertices;
	std::vector<Vec2> indexed_uvs;
	std::vector<Vec3> indexed_normals; 
	loadOBJ(path.c_str(), vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
	m_VertexCount = indexed_vertices.size();
	m_VertexArray = new stShapeVertices[m_VertexCount];
	m_IndexCount = indices.size();
	m_IndiceArray = new GLushort[m_IndexCount];
	m_PrimitiveType = PT_TRIANGLE_STRIP;
	m_IndexFormat = INDEX16;
	for (int i = 0; i < m_VertexCount; i++){
		m_VertexArray[i].Position = indexed_vertices.at(i);
		m_VertexArray[i].TexUV = indexed_uvs.at(i);
		m_VertexArray[i].Normal = indexed_normals.at(i);
	}
	for (int i = 0; i < m_IndexCount; i++){
		m_IndiceArray[i] = indices.at(i);
	}
	//绑定数据到VB中
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(stShapeVertices),
		m_VertexArray, GL_STATIC_DRAW);

	//IB
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount * sizeof(GLushort),
		m_IndiceArray, GL_STATIC_DRAW);
	BuildShader();
}

void C3DShape::CreateSkinLines()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);
	draw_type = SHAPE_3DMAX_BONEANIMATION;

	//创建顶点数组,这里共使用3个顶点
	m_SkinVertexArray = new stSkinVertices[3];
	//第一个顶点在（0, 20，0）的位置，它受第一根骨骼的影响，权重为100%，色彩为红色。
	m_SkinVertexArray[0].Position = Vec3(0, 20, 0);
	m_SkinVertexArray[0].Indices = Vec4(0.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[0].Blend = Vec4(1.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[0].Color = Vec4(1.0, 0.0, 0.0, 1.0);
	//第二个顶点在（0，0 ,0）的位置，它受第二根骨骼的影响，权重为100%，色彩为绿色。
	m_SkinVertexArray[1].Position = Vec3(0, 0, 0);
	m_SkinVertexArray[1].Indices = Vec4(1.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[1].Blend = Vec4(1.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[1].Color = Vec4(0.0, 1.0, 0.0, 1.0);
	//第三个顶点在（20，0 , 0）的位置，它受第二根骨骼的影响，权重为100%，色彩为蓝色。
	m_SkinVertexArray[2].Position = Vec3(20, 0, 0);
	m_SkinVertexArray[2].Indices = Vec4(2.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[2].Blend = Vec4(1.0, 0.0, 0.0, 0.0);
	m_SkinVertexArray[2].Color = Vec4(0.0, 0.0, 1.0, 1.0);


	//创建索引数组
	m_IndiceArray = new GLushort[3];
	m_IndiceArray[0] = 0;
	m_IndiceArray[1] = 1;
	m_IndiceArray[2] = 2;

	m_VertexCount = 3;
	m_IndexCount = 3;
	m_PrimitiveType = PT_LINE_STRIP;
	m_IndexFormat = INDEX16;
	 
	//绑定数据到VB中。
	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB,
		m_VertexCount * sizeof(stSkinVertices),
		m_SkinVertexArray,
		GL_STATIC_DRAW);

	//绑定数据到IB中。
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB,
		m_IndexCount*sizeof(GLushort),
		m_IndiceArray,
		GL_STATIC_DRAW);

	BuildShader();
}

void C3DShape::SetTexture(const char* texture, bool repeat)
{
	is_Draw_Texture = true;
	std::string strEx = "png";
	std::string str = texture;
	std::string::size_type pos = str.find_last_of(".");
	if(pos != std::string::npos){
		strEx = str.substr(pos+1);
	}else{return;}
	log("Ex:%s  image:%s",strEx.c_str(), texture);
	
	if(strEx == "dds" || strEx == "DDS"){
		TextureDDS = loadDDS(FileUtils::getInstance()->fullPathForFilename(texture).c_str());
		isTextureDDS = true;
		m_TextureFileName = texture;
		return;
	}
	Image *image = new Image;
	std::string imagePath = FileUtils::getInstance()->fullPathForFilename(texture);
	image->initWithImageFile(imagePath);
	m_Texture = new Texture2D();
	m_Texture->initWithImage(image);
	if (repeat){
		Texture2D::TexParams tRepeatParams;
		tRepeatParams.magFilter = GL_LINEAR;
		tRepeatParams.minFilter = GL_LINEAR;
		tRepeatParams.wrapS = GL_REPEAT;
		tRepeatParams.wrapT = GL_REPEAT;
		m_Texture->setTexParameters(&tRepeatParams);
		m_Texture->setAntiAliasTexParameters();
	}
	
	m_TextureFileName = texture;
	isTextureDDS = false;
	CC_SAFE_DELETE(image);
}

void C3DShape::SetCubeTexture(const char* szTextureFileName)
{

}

const char* C3DShape::GetTextureFileName()
{
	return m_TextureFileName.c_str();
}

void C3DShape::setBoneMatrixArray(Mat4* pBoneMatrixArray, int nBoneMatrixCount)
{
	m_BoneMatrixArray = pBoneMatrixArray;
	m_BoneMatrixCount = nBoneMatrixCount;
}

const BlendFunc& C3DShape::getBlendFunc()
{
	return m_BlendFunc;
}

void C3DShape::setBlendFunc(const BlendFunc& func)
{
	m_BlendFunc = func;
}

//创建Shader
void C3DShape::BuildShader()
{
	if (m_SkinVertexArray){
		m_ShaderProgram = GLProgram::createWithFilenames("Shaders/SkinShapeVS.vsh", "Shaders/SkinShapePS.fsh");
		m_ShaderProgram->addAttribute("a_position", GLProgram::VERTEX_ATTRIB_POSITION);
		m_ShaderProgram->addAttribute("a_normal", GLProgram::VERTEX_ATTRIB_NORMAL);
		m_ShaderProgram->addAttribute("a_texCoord", GLProgram::VERTEX_ATTRIB_TEX_COORD);
		m_ShaderProgram->addAttribute("a_blendWeight", GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
		m_ShaderProgram->addAttribute("a_blendIndex", GLProgram::VERTEX_ATTRIB_BLEND_INDEX);
		m_ShaderProgram->retain();
	}
	else{
		m_ShaderProgram = GLProgram::createWithFilenames("Shaders/C3DShapeVS.vsh", "Shaders/C3DShapePS.fsh");
		m_ShaderProgram->addAttribute("a_position", GLProgram::VERTEX_ATTRIB_POSITION);
		m_ShaderProgram->addAttribute("a_texCoord", GLProgram::VERTEX_ATTRIB_TEX_COORD);
		m_ShaderProgram->addAttribute("a_color", GLProgram::VERTEX_ATTRIB_COLOR);
		m_ShaderProgram->retain();
	}
}

void C3DShape::Render()
{
	if (false == IsVisible())return;
	if(m_ShaderProgram && m_VertexCount > 0){
		m_ShaderProgram->use();
		//更新shader
		m_ShaderProgram->setUniformsForBuiltins();
		if (m_SkinVertexArray){
			Render_SkinVertices();
		}
		else{
			Render_ShapeVertices();
		}
	}
	C3DNode::Render();
}

void C3DShape::Render_ShapeVertices()
{

	Mat4 matrixP;
	Mat4 matrixMV;
	Mat4 matrixMVP;
	Mat4 matrixWorld;
	Mat4 matrixMVPW;
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	GLuint matrixId = glGetUniformLocation(m_ShaderProgram->getProgram(), "CC_MVPMatrix");
	matrixWorld = GetWorldMatrix();
	kmMat4Multiply(&matrixMVPW, &matrixMVP, &matrixWorld);
	m_ShaderProgram->setUniformLocationWithMatrix4fv(matrixId, (GLfloat*)&matrixMVPW.m, 1);
	GLuint m_TextureId = glGetUniformLocation(m_ShaderProgram->getProgram(), "myTextureSampler");
	m_ShaderProgram->setUniformLocationWith1i(m_TextureId, 0);
	GLuint is_color_Id = glGetUniformLocation(m_ShaderProgram->getProgram(), "is_drawColor");
	m_ShaderProgram->setUniformLocationWith1i(is_color_Id, is_DrawColor);
	GLuint is_texture_Id = glGetUniformLocation(m_ShaderProgram->getProgram(), "is_drawTexture");
	m_ShaderProgram->setUniformLocationWith1i(is_texture_Id, is_Draw_Texture);
	if (is_Draw_Texture){
		if (isTextureDDS == true){
			GL::bindTexture2DN(0, TextureDDS);
			ccGLBindTexture2D(TextureDDS);
			ccGLBlendFunc(m_BlendFunc.src, m_BlendFunc.dst);
		}
		else{
			GL::bindTexture2DN(0, this->m_Texture->getName());
			ccGLBindTexture2D(m_Texture->getName());
			ccGLBlendFunc(m_BlendFunc.src, m_BlendFunc.dst);
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	/*	glEnable(GL_ARRAY_BUFFER);*/
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE,
		sizeof(stShapeVertices), (GLvoid*)offsetof(stShapeVertices, Position));
	if (is_Draw_Texture){
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE,
			sizeof(stShapeVertices), (GLvoid*)offsetof(stShapeVertices, TexUV));
	}
	if (is_DrawColor){
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE,
			sizeof(stShapeVertices), (GLvoid*)offsetof(stShapeVertices, Color));
	}

	//如果有索引缓冲
	if (m_IndexCount > 0 && m_IndiceArray){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		glEnable(GL_ELEMENT_ARRAY_BUFFER);
		glDrawElements(m_PrimitiveType, (GLsizei)m_IndexCount, m_IndexFormat, 0);
	}
	else{
		glDrawArrays(m_PrimitiveType, 0, m_VertexCount);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CC_INCREMENT_GL_DRAWS(1);
}

void C3DShape::Render_SkinVertices()
{
	kmMat4 matrixP;
	kmMat4 matrixMV;
	kmMat4 matrixMVP;
	Mat4 matrixWorld;
	Mat4 matrixMVPW;
	kmGLGetMatrix(KM_GL_PROJECTION, &matrixP);
	kmGLGetMatrix(KM_GL_MODELVIEW, &matrixMV);
	kmMat4Multiply(&matrixMVP, &matrixP, &matrixMV);
	GLuint matrixId = glGetUniformLocation(m_ShaderProgram->getProgram(), "CC_MVPMatrix");
	matrixWorld = GetWorldMatrix();
	kmMat4Multiply(&matrixMVPW, &matrixMVP, &matrixWorld);
	m_ShaderProgram->setUniformLocationWithMatrix4fv(matrixId, (GLfloat*)&matrixMVPW.m, 1);
	GLuint m_TextureId = glGetUniformLocation(m_ShaderProgram->getProgram(), "myTextureSampler");
	m_ShaderProgram->setUniformLocationWith1i(m_TextureId, 0);

	if (m_BoneMatrixCount > 0)
	{
		//GLuint matrixSize = glGetUniformLocation(m_ShaderProgram->getProgram(), "MAX_MATRIXSIZE");
		//m_ShaderProgram->setUniformLocationWith1i(matrixSize, m_BoneMatrixCount);
		GLuint matWorldId = glGetUniformLocation(m_ShaderProgram->getProgram(), "u_matrixPalette");
		m_ShaderProgram->setUniformLocationWithMatrix4fv(matWorldId, (GLfloat*)m_BoneMatrixArray, m_BoneMatrixCount);
	}
	GLuint is_texture_Id = glGetUniformLocation(m_ShaderProgram->getProgram(), "is_Draw_Texture");
	m_ShaderProgram->setUniformLocationWith1i(is_texture_Id, is_Draw_Texture);
	if (is_Draw_Texture){
		if (isTextureDDS == true){
			GL::bindTexture2DN(0, TextureDDS);
			ccGLBindTexture2D(TextureDDS);
			ccGLBlendFunc(m_BlendFunc.src, m_BlendFunc.dst);
		}
		else{
			GL::bindTexture2DN(0, this->m_Texture->getName());
			ccGLBindTexture2D(m_Texture->getName());
			ccGLBlendFunc(m_BlendFunc.src, m_BlendFunc.dst);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_VertexCount * sizeof(stSkinVertices), m_SkinVertexArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndexCount*sizeof(GLushort), m_IndiceArray, GL_STATIC_DRAW);

	// vertices
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(stSkinVertices), (GLvoid*)offsetof(stSkinVertices, Position));
	// normal
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_NORMAL);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(stSkinVertices), (GLvoid*)offsetof(stSkinVertices, Normal));
	// tex coords
	if (is_Draw_Texture){
		glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
		glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(stSkinVertices), (GLvoid*)offsetof(stSkinVertices, TexUV));
	}
	//  blend indices
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_BLEND_INDEX);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_BLEND_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof(stSkinVertices), (GLvoid*)offsetof(stSkinVertices, Indices));
	//  blend weight
	glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_BLEND_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(stSkinVertices), (GLvoid*)offsetof(stSkinVertices, Blend));

	// Draw
	glDrawElements(m_PrimitiveType, (GLsizei)m_IndexCount, m_IndexFormat, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	CC_INCREMENT_GL_DRAWS(1);
}

void C3DShape::Release()
{
	if(m_VertexArray){
		delete[] m_VertexArray;
		m_VertexArray = NULL;
	}
	if (m_SkinVertexArray)
	{
		delete[] m_SkinVertexArray;
		m_SkinVertexArray = NULL;
	}
	if(m_IndiceArray)
	{
		delete[] m_IndiceArray;
		m_IndiceArray = NULL;
	}
	m_VertexCount = 0;
	m_IndexCount = 0;
	if(glIsBuffer(m_VertexBuffer)){
		glDeleteBuffers(1, &m_VertexBuffer);
		m_VertexBuffer = 0;
	}
	if(glIsBuffer(m_IndexBuffer)){
		glDeleteBuffers(1, &m_IndexBuffer);
		m_IndexBuffer = 0;
	}
}