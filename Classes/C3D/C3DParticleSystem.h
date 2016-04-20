#pragma once
#include "C3DShape.h"

struct st3DParticle{
	//λ��
	Vec3 m_Position;
	//�ٶ�
	Vec3 m_Velocity;
	//���ٶ�
	Vec3 m_Acceleration;
	//��С
	Vec3 m_Size;
	//��С�仯�ٶ�
	Vec3 m_Size_Velocity;
	//ɫ��
	Vec3 m_Color;
	//ɫ�ʱ仯�ٶ�
	Vec3 m_Color_Velocity;
	float m_Life;
	//�����仯�ٶ�
	float m_Life_Velocity;
};

struct st3DParticleData{
	Vec3 m_Position_Min;
	Vec3 m_Position_Max;
	Vec3 m_Velocity_Min;
	Vec3 m_Velocity_Max;
	Vec3 m_Acceleration_Min;
	Vec3 m_Acceleration_Max;
	Vec3 m_Size_Min;
	Vec3 m_Size_Max;
	Vec3 m_Size_Velocity;
	Vec3 m_Color_Min;
	Vec3 m_Color_Max;
	Vec3 m_Color_Velocity;
	float m_Life_Min;
	float m_Life_Max;
	float m_Life_Velocity;
};

enum eParticleType
{
	PT_FIRE,
	PT_FIREWORK,
	PT_FOUNTAIN,
	PT_MAX,
};

class C3DParticleSystem : public C3DShape
{
public:
	C3DParticleSystem(void);
	~C3DParticleSystem(void);
public:
	void InitPS_Fire(int v_TotalSize);
	void InitPS_FireWork(int v_TotalSize);
	void InitPS_Fountain(int v_TotalSize);
	void SetParticleData_FireWork(st3DParticleData& vParticle);
	void SetParticleData_Fire(st3DParticleData& vParticle);
	void SetParticleData_Fountain(st3DParticleData& vParticle);
	void addPS(float dt);
	virtual void Update();
	virtual void Render();

	void resetParticleInitPos(Vec3 pos);
	st3DParticleData getParticleData(){ return m_sParticleData; };
	eParticleType getParticleType(){ return m_eParticleType; };

private:
	void InitParticle(st3DParticle& vParticle);
	void BuildVBIB();
protected:
	//��������
	st3DParticleData m_sParticleData;
	std::vector<st3DParticle> m_3DParticleVec;
	//��ʼʱ��
	timeval m_StartTime;
	timeval m_PreTime;
	double sumTime;
	//��һ�γ���
	bool m_bFirstRender;
	eParticleType m_eParticleType;
	float updateTime;
	int m_createSize;
	double randomVelocity;
};

