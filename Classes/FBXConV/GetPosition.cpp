#include "GetPosition.h"

// Get the global position of the node for the current pose.
// If the specified node is not part of the pose or no pose is specified, get its
// global position at the current time.
//获取当前节点相对于当前动作的全局位置，假如当前节点不是姿势的一部分，那么久获取到他的世界坐标在当前帧中
FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, FbxPose* pPose, FbxAMatrix* pParentGlobalPosition)
{
	FbxAMatrix lGlobalPosition;
	bool        lPositionFound = false;

	if (pPose)
	{
		int lNodeIndex = pPose->Find(pNode);

		if (lNodeIndex > -1)
		{
			// The bind pose is always a global matrix.
			// If we have a rest pose, we need to check if it is
			// stored in global or local space.
			//bindPose是一个全局的
			//假如有一个静止的动作，需要检查是否存储在全局或本地空间
			if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
			{
				lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
			}
			else
			{
				// We have a local matrix, we need to convert it to
				// a global space matrix.
				//转换成全局的矩阵
				FbxAMatrix lParentGlobalPosition;

				if (pParentGlobalPosition)
				{
					lParentGlobalPosition = *pParentGlobalPosition;
				}
				else
				{
					if (pNode->GetParent())
					{
						lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
					}
				}

				FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}

			lPositionFound = true;
		}
	}

	if (!lPositionFound)
	{
		// There is no pose entry for that node, get the current global position instead.

		// Ideally this would use parent global position and local position to compute the global position.
		// Unfortunately the equation 
		//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
		// does not hold when inheritance type is other than "Parent" (RSrs).
		// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
		//计算全局位置
		lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
	}

	return lGlobalPosition;
}

// Get the matrix of the given pose
FbxAMatrix GetPoseMatrix(FbxPose* pPose, int pNodeIndex)
{
	FbxAMatrix lPoseMatrix;
	FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

// Get the geometry offset to a node. It is never inherited by the children.
//获取节点的几何偏移
FbxAMatrix GetGeometry(FbxNode* pNode)
{
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

	return FbxAMatrix(lT, lR, lS);
}

