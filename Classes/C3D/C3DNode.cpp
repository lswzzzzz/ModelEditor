#include "C3DNode.h"

unsigned long C3DNode::m_NodeCount = 0;
//构造
C3DNode::C3DNode():
m_pParent(NULL),
m_pFirstChild(NULL),
m_pFirstBorther(NULL),
m_bVisible(true)
{
	//名称
	sprintf(m_szName,"Node%d",C3DNode::m_NodeCount++);

	//自身
	m_Translate_Self = Vec3::ZERO;
	m_Scale_Self = Vec3::ONE;
	m_Rotate_Self.x = m_Rotate_Self.y = m_Rotate_Self.z = m_Rotate_Self.w = 0.0;
	//相对父节点
	m_Translate_Parent = Vec3::ZERO;
	m_Scale_Parent = Vec3::ONE;
	m_Rotate_Parent.x = m_Rotate_Parent.y = m_Rotate_Parent.z = m_Rotate_Parent.w = 0.0;
}
//析构
C3DNode::~C3DNode()
{

}
//增加子节点
void		C3DNode::AddChild(C3DNode*	pNode)
{
	if(m_pFirstChild)
	{
		m_pFirstChild->AddBorther(pNode);
		pNode->SetParent(this);
		return ;
	}
	m_pFirstChild = pNode;
	pNode->SetParent(this);
}


//增加兄弟节点
void		C3DNode::AddBorther(C3DNode*	pNode)
{
	if(m_pFirstBorther)
	{
		m_pFirstBorther->AddBorther(pNode);
		pNode->SetParent(this);
		return ;
	}
	m_pFirstBorther = pNode;
	pNode->SetParent(this);
}

//设置父节点
void		C3DNode::SetParent(C3DNode*	pNode)
{
	m_pParent = pNode ;
}
//查询子节点
C3DNode*	C3DNode::QueryNode(const char* szName,bool bWithBorther)
{
	if( 0 == strcmp(m_szName,szName) )
	{
		return this;
	}
	if(m_pFirstChild)
	{
		C3DNode*	pFindNode = m_pFirstChild->QueryNode(szName,true);
		if(pFindNode)
		{
			return pFindNode;
		}
	}
	if(bWithBorther)
	{
		if(m_pFirstBorther)
		{
			C3DNode*	pFindNode = m_pFirstBorther->QueryNode(szName,true);
			if(pFindNode)
			{
				return pFindNode;
			}
		}
	}
	return NULL;
}
//删除子节点
bool		C3DNode::DelNode(C3DNode*	pNode,bool bWithBorther)
{
	if(m_pFirstChild)
	{
		if(m_pFirstChild == pNode)
		{
			C3DNode*	pBorther = m_pFirstChild->m_pFirstBorther;
			delete m_pFirstChild ;
			m_pFirstChild = pBorther;
			return true;
		}
		if(true == m_pFirstChild->DelNode(pNode,true))
		{
			return true;
		}
	}

	if(bWithBorther)
	{
		if(m_pFirstBorther == pNode)
		{
			C3DNode*	pBorther = m_pFirstBorther->m_pFirstBorther;
			delete m_pFirstBorther ;
			m_pFirstBorther = pBorther;
			return true;
		}
		if(true == m_pFirstBorther->DelNode(pNode,true))
		{
			return true;
		}
	}

	return false;
}
//更新
void		C3DNode::Update()
{
	if(m_pFirstChild)
	{
		m_pFirstChild->Update();
	}
	if(m_pFirstBorther)
	{
		m_pFirstBorther->Update();
	}
}
//渲染
void		C3DNode::Render()
{
	if(m_pFirstChild)
	{
		m_pFirstChild->Render();
		
	}
	if(m_pFirstBorther)
	{
		m_pFirstBorther->Render();
	}
}
//设置名称
void		C3DNode::SetName(const char* szName)
{
	strcpy(m_szName,szName);
}
//取得名称
const char* C3DNode::GetName()
{
	return m_szName;
}
//设置显示
void		C3DNode::SetVisible(bool bVisible)
{
	m_bVisible = bVisible ;
}
//是否显示
bool		C3DNode::IsVisible()
{
	return m_bVisible;
}
//设置对自身中心点的偏移
void	C3DNode::SetTranslate(float x,float y,float z)
{
	m_Translate_Self.x = x;
	m_Translate_Self.y = y;
	m_Translate_Self.z = z;
}
//取得对自身中心点的偏移
const Vec3&		C3DNode::GetTranslate()
{
	return m_Translate_Self;
}

//设置相对自身中心点的缩放
void	C3DNode::SetScale(float x,float y,float z)
{
	m_Scale_Self.x = x;
	m_Scale_Self.y = y;
	m_Scale_Self.z = z;
}
//取得相对自身中心点的缩放
const Vec3&		C3DNode::GetScale()
{
	return m_Scale_Self;
}

//设置相对自身中心点的旋转
void	C3DNode::SetRotate(float x,float y,float z,float w)
{
	m_Rotate_Self.x = x;
	m_Rotate_Self.y = y;
	m_Rotate_Self.z = z;
	m_Rotate_Self.w = w;
}
//取得相对自身中心点的旋转
const Quaternion& C3DNode::GetRotate()
{
	return m_Rotate_Self;
}

//设置相对父节点的偏移
void	C3DNode::SetTranslate_Parent(float x,float y,float z)
{
	m_Translate_Parent.x = x;
	m_Translate_Parent.y = y;
	m_Translate_Parent.z = z;
}
//取得相对父节点的偏移
const Vec3&		C3DNode::GetTranslate_Parent()
{
	return m_Translate_Parent;
}

//设置相对父节点的缩放
void	C3DNode::SetScale_Parent(float x,float y,float z)
{
	m_Scale_Parent.x = x;
	m_Scale_Parent.y = y;
	m_Scale_Parent.z = z;
}
//取得相对父节点的缩放
const Vec3&		C3DNode::GetScale_Parent()
{
	return m_Scale_Parent;
}

//设置相对父节点的旋转
void	C3DNode::SetRotate_Parent(float x,float y,float z,float w)
{
	m_Rotate_Parent.x = x;
	m_Rotate_Parent.y = y;
	m_Rotate_Parent.z = z;
	m_Rotate_Parent.w = w;
}

//取得相对父节点的旋转
const Quaternion&  C3DNode::GetRotate_Parent()
{
	return m_Rotate_Parent;
}
//取得自身的变换矩阵
Mat4	C3DNode::GetMatrix_Self()
{
	Mat4	tResultMatrix = Mat4::IDENTITY;

	Mat4	tRotateMatrix;
	Vec3	tAxis = Vec3(m_Rotate_Self.x,m_Rotate_Self.y,m_Rotate_Self.z);
	Mat4::createRotation(tAxis,m_Rotate_Self.w,&tRotateMatrix);

	Mat4	tScaleMatrix;
	Mat4::createScale(m_Scale_Self,&tScaleMatrix);

	Mat4	tTranslateMatrix;
	Mat4::createTranslation(m_Translate_Self,&tTranslateMatrix);

	tResultMatrix = tScaleMatrix ;
	tResultMatrix.multiply(tRotateMatrix);
	tResultMatrix.multiply(tTranslateMatrix);
	return tResultMatrix;
}
//取得相对父结点的相对矩阵
Mat4	C3DNode::GetMatrix_Parent()
{
	Mat4	tResultMatrix = Mat4::IDENTITY;

	Mat4	tRotateMatrix;
	Vec3	tAxis = Vec3(m_Rotate_Parent.x,m_Rotate_Parent.y,m_Rotate_Parent.z);
	Mat4::createRotation(tAxis,m_Rotate_Parent.w,&tRotateMatrix);

	Mat4	tScaleMatrix;
	Mat4::createScale(m_Scale_Parent,&tScaleMatrix);


	Mat4	tTranslateMatrix;
	Mat4::createTranslation(m_Translate_Parent,&tTranslateMatrix);

	tResultMatrix = tScaleMatrix ;
	tResultMatrix.multiply(tRotateMatrix);
	tResultMatrix.multiply(tTranslateMatrix);

	if(m_pParent)
	{ 
		Mat4	tMatrix_ParentNode = m_pParent->GetWorldMatrix();
		tMatrix_ParentNode.multiply(tResultMatrix);
		return tMatrix_ParentNode;
	}
	return tResultMatrix;
}
//取得世界矩阵
Mat4	C3DNode::GetWorldMatrix()
{
	Mat4	tMatrix1 = GetMatrix_Self();
	Mat4	tMatrix2 = GetMatrix_Parent();
	tMatrix2.multiply(tMatrix1);
	return  tMatrix2;

}