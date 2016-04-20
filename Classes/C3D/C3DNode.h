#ifndef _C3DNODE_H
#define _C3DNODE_H


#include "cocos2d.h"
USING_NS_CC;

class C3DNode
{
public:
	//�ڵ������
	static	unsigned long m_NodeCount;
	//����
	C3DNode();
	//����
	virtual ~C3DNode();
public:
	//�����ӽڵ�
	void		AddChild(C3DNode*	pNode);
	//�����ֵܽڵ�
	void		AddBorther(C3DNode*	pNode);
	//���ø��ڵ�
	void		SetParent(C3DNode*	pNode);
	//��ѯ�ӽڵ�
	C3DNode*	QueryNode(const char* szName,bool bWithBorther = false);
	//ɾ���ӽڵ�
	bool		DelNode(C3DNode*	pNode,bool bWithBorther = false);
public:
	//����
	virtual	void	Update();
	//��Ⱦ
	virtual	void	Render();
public:

	//��������
	virtual void		SetName(const char* szName);
	//ȡ������
	virtual const char* GetName();

	//������ʾ
	void		SetVisible(bool bVisible);
	//�Ƿ���ʾ
	bool		IsVisible();
	
	//��������������ĵ��ƫ��
	void			SetTranslate(float x,float y,float z);
	//ȡ������������ĵ��ƫ��
	const Vec3&		GetTranslate();

	//��������������ĵ������
	void			SetScale(float x,float y,float z);
	//ȡ������������ĵ������
	const Vec3&		GetScale();

	//��������������ĵ����ת
	void			SetRotate(float x,float y,float z,float angle);
	//ȡ������������ĵ����ת
	const Quaternion& GetRotate();

	//������Ը��ڵ��ƫ��
	void			SetTranslate_Parent(float x,float y,float z);
	//ȡ����Ը��ڵ��ƫ��
	const Vec3&		GetTranslate_Parent();

	//������Ը��ڵ������
	void			SetScale_Parent(float x,float y,float z);
	//ȡ����Ը��ڵ������
	const Vec3&		GetScale_Parent();

	//������Ը��ڵ����ת
	void			SetRotate_Parent(float x,float y,float z,float angle);
	//ȡ����Ը��ڵ����ת
	const Quaternion& GetRotate_Parent();
	
	//ȡ������ı任����
	Mat4			GetMatrix_Self();
	//ȡ����Ը�������Ծ���
	Mat4			GetMatrix_Parent();
	//ȡ���������
	Mat4			GetWorldMatrix();

	
protected:
	//���ڵ�
	C3DNode*	m_pParent;
	//�׸��ӽ��
	C3DNode*	m_pFirstChild;
	//�׸��ֵܽڵ�
	C3DNode*	m_pFirstBorther;

	//��ǰ�ڵ�����
	char		m_szName[32];
	//�Ƿ���ʾ
	bool		m_bVisible;
	
	//����������ĵ��λ��ƫ��
	Vec3		m_Translate_Self;
	//�����������ĵ������
	Vec3		m_Scale_Self;
	//�����������ĵ����ת
	Quaternion	m_Rotate_Self;

	//��Ը��ڵ��λ��ƫ��
	Vec3		m_Translate_Parent;
	//�����������ĵ������
	Vec3		m_Scale_Parent;
	//��Ը��ڵ����ת
	Quaternion	m_Rotate_Parent;
	
}
;

#endif