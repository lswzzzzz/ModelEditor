#include "C3DParticleSystem.h"
#include "Global.h"

C3DParticleSystem::C3DParticleSystem(void)
{
	m_bFirstRender = true;
	m_createSize = 0;
	updateTime = 0.0f;
	randomVelocity = CCRANDOM_0_1();
}

C3DParticleSystem::~C3DParticleSystem(void)
{
}

void C3DParticleSystem::SetParticleData_FireWork(st3DParticleData& vParticle)
{
	vParticle.m_Position_Min = Vec3::ZERO;
	vParticle.m_Position_Max = Vec3::ZERO;
	vParticle.m_Velocity_Min = -30.0f * Vec3::ONE;
	vParticle.m_Velocity_Max = 30.0f * Vec3::ONE;
	vParticle.m_Acceleration_Min = Vec3::ZERO;
	vParticle.m_Acceleration_Max = Vec3::ZERO;
	vParticle.m_Size_Min = 5.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Max = 8.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Velocity = Vec3(0, 0, 0);
	vParticle.m_Color_Min = Vec3(0, 0, 0);
	vParticle.m_Color_Max = Vec3(1.0f, 1.0f, 1.0f);
	vParticle.m_Color_Velocity = Vec3(0, 0, 0);
	vParticle.m_Life_Min = 0.0f;
	vParticle.m_Life_Max = 100.0f;
	vParticle.m_Life_Velocity = randomVelocity * -50.0f;
}

void C3DParticleSystem::SetParticleData_Fire(st3DParticleData& vParticle)
{
	vParticle.m_Position_Min = Vec3::ZERO;
	vParticle.m_Position_Max = Vec3::ZERO;
	vParticle.m_Velocity_Min = Vec3(0, 5.0f, 0);
	vParticle.m_Velocity_Max = Vec3(0, 28.0f, 0);
	vParticle.m_Acceleration_Min = Vec3(0, 1.0f, 0);
	vParticle.m_Acceleration_Max = Vec3(0, 5.0f, 0);
	vParticle.m_Size_Min = 4.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Max = 8.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Velocity = Vec3(0, 0, 0);
	vParticle.m_Color_Min = Vec3(0.5, 0, 0);
	vParticle.m_Color_Max = Vec3(1.0f, 0, 0);
	vParticle.m_Color_Velocity = Vec3(-0.5, 0, 0);
	vParticle.m_Life_Min = 0.0f;
	vParticle.m_Life_Max = 200.0f;
	vParticle.m_Life_Velocity = randomVelocity * -50.0f;
}

void C3DParticleSystem::SetParticleData_Fountain(st3DParticleData& vParticle)
{
	vParticle.m_Position_Min = Vec3::ZERO;
	vParticle.m_Position_Max = Vec3::ZERO;
	vParticle.m_Velocity_Min = Vec3(-8.0f, 30.0f, 0);
	vParticle.m_Velocity_Max = Vec3(8.0f, 30.0f, 0);
	vParticle.m_Acceleration_Min = Vec3(-3.0f, -10.0f, 0);
	vParticle.m_Acceleration_Max = Vec3(3.0f, -10.0f, 0);
	vParticle.m_Size_Min = 4.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Max = 8.0f * Vec3(1.0, 1.0, 1.0);
	vParticle.m_Size_Velocity = Vec3(0, 0, 0);
	vParticle.m_Color_Min = Vec3(0.5, 0.7, 0.6);
	vParticle.m_Color_Max = Vec3(1, 1.0, 1.0);
	vParticle.m_Color_Velocity = Vec3(0, 0, 0);
	vParticle.m_Life_Min = 0.0f;
	vParticle.m_Life_Max = 30.0f;
	vParticle.m_Life_Velocity = randomVelocity * -48.0f - 2.0f;
}

void C3DParticleSystem::InitPS_Fire(int v_TotalSize)
{
	m_3DParticleVec.clear();  
	m_createSize = v_TotalSize;
	SetParticleData_Fire(m_sParticleData);
	for (int i = 0; i < v_TotalSize; i++){
		st3DParticle t3DParticle;
		InitParticle(t3DParticle);
		m_3DParticleVec.push_back(t3DParticle);
	}
	BuildVBIB();
	m_eParticleType = PT_FIRE;
	m_BlendFunc = BlendFunc::ADDITIVE;
	m_bFirstRender = true;
	updateTime = 5.0f;
}

void C3DParticleSystem::InitPS_FireWork(int v_TotalSize)
{
	m_3DParticleVec.clear();
	m_createSize = v_TotalSize;
	SetParticleData_FireWork(m_sParticleData);
	for (int i = 0; i < v_TotalSize; i++){
		st3DParticle t3DParticle;
		InitParticle(t3DParticle);
		m_3DParticleVec.push_back(t3DParticle);
	}
	BuildVBIB();
	m_eParticleType = PT_FIREWORK;
	m_BlendFunc = BlendFunc::ADDITIVE;
	m_bFirstRender = true;
	updateTime = 5.0f;
}

void C3DParticleSystem::InitPS_Fountain(int v_TotalSize)
{
	m_3DParticleVec.clear();
	m_createSize = v_TotalSize;
	SetParticleData_Fountain(m_sParticleData);
	for (int i = 0; i < v_TotalSize; i++){
		st3DParticle t3DParticle;
		InitParticle(t3DParticle);
		m_3DParticleVec.push_back(t3DParticle);
	}
	BuildVBIB();
	m_eParticleType = PT_FOUNTAIN;
	m_BlendFunc = BlendFunc::ADDITIVE;
	m_bFirstRender = true;
	updateTime = 0.0f;
}

void C3DParticleSystem::addPS(float dt)
{
	for (int i = 0; i < m_createSize; i++){
		st3DParticle t3DParticle;
		InitParticle(t3DParticle);
		m_3DParticleVec.push_back(t3DParticle);
	}
	BuildVBIB();
	sumTime = 0.0;
}

void C3DParticleSystem::Update()
{
	//如果是第一次更新记录下时间
	if(m_bFirstRender){
		gettimeofday(&m_StartTime, 0);
		m_PreTime = m_StartTime;
		sumTime = 0.0;
		m_bFirstRender = false;
	}
	//取得当前时间，计算Delay
	timeval t;
	gettimeofday(&t, 0);
	double delay = 1000.0f * (t.tv_sec - m_PreTime.tv_sec) + 0.001f * (t.tv_usec - m_PreTime.tv_usec);
	if(delay > 0.0){
		delay = 0.001f * delay;
		sumTime += delay;
		if (sumTime > updateTime){
			addPS(sumTime);
		}
		Vec3 tMinColor = Vec3::ZERO;
		Vec3 tMaxColor = Vec3::ONE;
		std::vector<st3DParticle>::iterator tIter;
		for(int i = m_3DParticleVec.size()-1;  i >= 0; i--){
			tIter = m_3DParticleVec.begin() + i;
			tIter->m_Velocity += tIter->m_Acceleration * delay;
			tIter->m_Position += tIter->m_Velocity * delay;
			tIter->m_Size += tIter->m_Size_Velocity * delay;
			tIter->m_Color += tIter->m_Color_Velocity * delay;
			tIter->m_Color.clamp(tMinColor, tMaxColor);
			tIter->m_Life += tIter->m_Life_Velocity * delay;
			if(tIter->m_Life < 0){
				m_3DParticleVec.erase(tIter);
			}
		}
		//根据粒子来生成相应的四边形顶点结构
		int nIndex = 0;
		for(tIter = m_3DParticleVec.begin(); tIter != m_3DParticleVec.end(); tIter++){
			m_VertexArray[nIndex].Position = tIter->m_Position + Vec3(-tIter->m_Size.x, tIter->m_Size.y, 0) * 0.5;
			m_VertexArray[nIndex].Color = tIter->m_Color;
			m_VertexArray[nIndex].TexUV = Vec2(0, 0);
			nIndex++;

			m_VertexArray[nIndex].Position= tIter->m_Position + Vec3(tIter->m_Size.x, tIter->m_Size.y, 0) * 0.5;
			m_VertexArray[nIndex].Color = tIter->m_Color;
			m_VertexArray[nIndex].TexUV = Vec2(1, 0);
			nIndex++;

			m_VertexArray[nIndex].Position= tIter->m_Position + Vec3(-tIter->m_Size.x, -tIter->m_Size.y, 0) * 0.5;
			m_VertexArray[nIndex].Color = tIter->m_Color;
			m_VertexArray[nIndex].TexUV = Vec2(0, 1);
			nIndex++;

			m_VertexArray[nIndex].Position= tIter->m_Position + Vec3(tIter->m_Size.x, -tIter->m_Size.y, 0) * 0.5;
			m_VertexArray[nIndex].Color = tIter->m_Color;
			m_VertexArray[nIndex].TexUV = Vec2(1, 1);
			nIndex++;
		}
		//绑定数据到VB中
		glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices), m_VertexArray, GL_DYNAMIC_DRAW);
		m_PreTime = t;
	}
}

void C3DParticleSystem::Render()
{
	//粒子系统渲染，如果是图片，关闭深度测试，否则屏幕重叠时会有ALPHA深度错误
	//Director::getInstance()->setDepthTest(false);

	C3DShape::Render();

	//Director::getInstance()->setDepthTest(true);
}

//初始化粒子
void C3DParticleSystem::InitParticle(st3DParticle& vParticle)
{
	Vec3 temp = (m_sParticleData.m_Position_Max - m_sParticleData.m_Position_Min);
	vParticle.m_Position = m_sParticleData.m_Position_Min + Vec3(temp.x * CCRANDOM_0_1(), temp.y*CCRANDOM_0_1(), temp.z*CCRANDOM_0_1());
	temp = (m_sParticleData.m_Velocity_Max - m_sParticleData.m_Velocity_Min);
	vParticle.m_Velocity = m_sParticleData.m_Velocity_Min + Vec3(temp.x * CCRANDOM_0_1(), temp.y * CCRANDOM_0_1(), temp.z * CCRANDOM_0_1());
	temp = (m_sParticleData.m_Acceleration_Max - m_sParticleData.m_Acceleration_Min);
	vParticle.m_Acceleration = m_sParticleData.m_Acceleration_Min + Vec3(temp.x*CCRANDOM_0_1(), temp.y*CCRANDOM_0_1(), temp.z*CCRANDOM_0_1());
	temp = (m_sParticleData.m_Size_Max - m_sParticleData.m_Size_Min);
	vParticle.m_Size = m_sParticleData.m_Size_Min + Vec3(temp.x*CCRANDOM_0_1(), temp.y*CCRANDOM_0_1(), temp.z*CCRANDOM_0_1());
	vParticle.m_Size_Velocity = m_sParticleData.m_Size_Velocity;
	temp = (m_sParticleData.m_Color_Max - m_sParticleData.m_Color_Min);
	vParticle.m_Color = m_sParticleData.m_Color_Min + Vec3(temp.x*CCRANDOM_0_1(), temp.y*CCRANDOM_0_1(), temp.z*CCRANDOM_0_1());
	vParticle.m_Color_Velocity = m_sParticleData.m_Color_Velocity;
	vParticle.m_Life = m_sParticleData.m_Life_Min + (m_sParticleData.m_Life_Max - m_sParticleData.m_Life_Min) * CCRANDOM_0_1();
	vParticle.m_Life_Velocity = m_sParticleData.m_Life_Velocity;
}

void C3DParticleSystem::BuildVBIB()
{
	Release();
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//创建VB与IB
	glGenBuffers(1, &m_VertexBuffer);
	glGenBuffers(1, &m_IndexBuffer);

	m_VertexCount = m_3DParticleVec.size() * 4;

	//创建数组并填充数据
	m_VertexArray = new stShapeVertices[m_VertexCount];
	std::vector<st3DParticle>::iterator tIter;
	int nIndex= 0;
	for(tIter=m_3DParticleVec.begin(); tIter != m_3DParticleVec.end(); tIter++){
		m_VertexArray[nIndex].Position = tIter->m_Position + Vec3(-tIter->m_Size.x, tIter->m_Size.y, 0) * 0.5;
		m_VertexArray[nIndex].Color = tIter->m_Color;
		m_VertexArray[nIndex].TexUV = Vec2(0, 0);
		nIndex++;

		m_VertexArray[nIndex].Position = tIter->m_Position + Vec3(tIter->m_Size.x, tIter->m_Size.y, 0) * 0.5;
		m_VertexArray[nIndex].Color = tIter->m_Color;
		m_VertexArray[nIndex].TexUV = Vec2(1, 0);
		nIndex++;

		m_VertexArray[nIndex].Position = tIter->m_Position + Vec3(-tIter->m_Size.x, -tIter->m_Size.y, 0) * 0.5;
		m_VertexArray[nIndex].Color = tIter->m_Color;
		m_VertexArray[nIndex].TexUV = Vec2(0, 1);
		nIndex++;

		m_VertexArray[nIndex].Position = tIter->m_Position + Vec3(tIter->m_Size.x, -tIter->m_Size.y, 0) * 0.5;
		m_VertexArray[nIndex].Color = tIter->m_Color;
		m_VertexArray[nIndex].TexUV = Vec2(1, 1);
		nIndex++;
	}
	m_IndexCount = m_3DParticleVec.size() * 6;
	//创建索引数组
	m_IndiceArray = new GLushort[m_IndexCount];
	//填充绘制四边形所需要的索引值
	int nParticleIndex = 0;
	nIndex = 0;
	for(tIter = m_3DParticleVec.begin(); tIter != m_3DParticleVec.end(); tIter++){
		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 0;
		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 1;
		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 2;

		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 1;
		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 2;
		m_IndiceArray[nIndex++] = nParticleIndex * 4 + 3;
		nParticleIndex++;
	}
	m_PrimitiveType = PT_TRIANGLES;
	m_IndexFormat = INDEX16;
	//绑定数据到VB中
	glBindBuffer(GL_ARRAY_BUFFER_ARB, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER_ARB, m_VertexCount * sizeof(stShapeVertices), m_VertexArray, GL_STATIC_DRAW);

	//绑定数据到IB中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER_ARB, m_IndexCount * sizeof(GLushort), m_IndiceArray, GL_STATIC_DRAW);

	BuildShader();
}

void C3DParticleSystem::resetParticleInitPos(Vec3 pos)
{
	m_sParticleData.m_Position_Min = Vec3(pos.x / GetScale().x, pos.y / GetScale().y, pos.z / GetScale().z);
	m_sParticleData.m_Position_Max = Vec3(pos.x / GetScale().x, pos.y / GetScale().y, pos.z / GetScale().z);
}
