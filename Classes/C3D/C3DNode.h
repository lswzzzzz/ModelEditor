#ifndef _C3DNODE_H
#define _C3DNODE_H


#include "cocos2d.h"
USING_NS_CC;

class C3DNode
{
public:
	//节点计数器
	static	unsigned long m_NodeCount;
	//构造
	C3DNode();
	//析构
	virtual ~C3DNode();
public:
	//增加子节点
	void		AddChild(C3DNode*	pNode);
	//增加兄弟节点
	void		AddBorther(C3DNode*	pNode);
	//设置父节点
	void		SetParent(C3DNode*	pNode);
	//查询子节点
	C3DNode*	QueryNode(const char* szName,bool bWithBorther = false);
	//删除子节点
	bool		DelNode(C3DNode*	pNode,bool bWithBorther = false);
public:
	//更新
	virtual	void	Update();
	//渲染
	virtual	void	Render();
public:

	//设置名称
	virtual void		SetName(const char* szName);
	//取得名称
	virtual const char* GetName();

	//设置显示
	void		SetVisible(bool bVisible);
	//是否显示
	bool		IsVisible();
	
	//设置相对自身中心点的偏移
	void			SetTranslate(float x,float y,float z);
	//取得相对自身中心点的偏移
	const Vec3&		GetTranslate();

	//设置相对自身中心点的缩放
	void			SetScale(float x,float y,float z);
	//取得相对自身中心点的缩放
	const Vec3&		GetScale();

	//设置相对自身中心点的旋转
	void			SetRotate(float x,float y,float z,float angle);
	//取得相对自身中心点的旋转
	const Quaternion& GetRotate();

	//设置相对父节点的偏移
	void			SetTranslate_Parent(float x,float y,float z);
	//取得相对父节点的偏移
	const Vec3&		GetTranslate_Parent();

	//设置相对父节点的缩放
	void			SetScale_Parent(float x,float y,float z);
	//取得相对父节点的缩放
	const Vec3&		GetScale_Parent();

	//设置相对父节点的旋转
	void			SetRotate_Parent(float x,float y,float z,float angle);
	//取得相对父节点的旋转
	const Quaternion& GetRotate_Parent();
	
	//取得自身的变换矩阵
	Mat4			GetMatrix_Self();
	//取得相对父结点的相对矩阵
	Mat4			GetMatrix_Parent();
	//取得世界矩阵
	Mat4			GetWorldMatrix();

	
protected:
	//父节点
	C3DNode*	m_pParent;
	//首个子结点
	C3DNode*	m_pFirstChild;
	//首个兄弟节点
	C3DNode*	m_pFirstBorther;

	//当前节点名称
	char		m_szName[32];
	//是否显示
	bool		m_bVisible;
	
	//相对自身中心点的位置偏移
	Vec3		m_Translate_Self;
	//自身自身中心点的缩放
	Vec3		m_Scale_Self;
	//自身自身中心点的旋转
	Quaternion	m_Rotate_Self;

	//相对父节点的位置偏移
	Vec3		m_Translate_Parent;
	//自身自身中心点的缩放
	Vec3		m_Scale_Parent;
	//相对父节点的旋转
	Quaternion	m_Rotate_Parent;
	
}
;

#endif