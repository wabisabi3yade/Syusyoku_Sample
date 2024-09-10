#include "pch.h"
#include "Bone.h"

using namespace DirectX::SimpleMath;

void Bone::CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx)
{	
	combinationMatrix = offsetMatrix * GetAnimMtx() * _parentMtx;
}

void Bone::CreateGlobalMtx(const DirectX::SimpleMath::Matrix& _parentMtx, const DirectX::SimpleMath::Matrix& _offsetMtx)
{
	globalMatrix = GetAnimMtx() * _parentMtx * _offsetMtx;
}

void Bone::SetBoneName(const std::string& _boneName)
{
	boneName = _boneName;
}

void Bone::SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix)
{
	offsetMatrix = _offsetMatrix;
}

void Bone::SetAnimTransform(const BoneTransform& _transform)
{
	animationTransform = _transform;
}

void Bone::SetIndex(u_int _idx)
{
	boneIdx = _idx;
}

std::string Bone::GetBoneName() const
{
	return boneName;
}

const DirectX::SimpleMath::Matrix& Bone::GetCombMtx() const
{
	return combinationMatrix;
}
 
DirectX::SimpleMath::Matrix Bone::GetAnimMtx() const
{
	// アニメーション行列を作成
	Matrix scaleMtx = Matrix::CreateScale(animationTransform.scale);
	Matrix rotationMtx = Matrix::CreateFromQuaternion(animationTransform.rotation);
	Matrix transformMtx = Matrix::CreateTranslation(animationTransform.position);
	Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

	return animationMtx;
}

const DirectX::SimpleMath::Matrix& Bone::GetOffsetMtx() const
{
	return offsetMatrix;
}

const DirectX::SimpleMath::Matrix& Bone::GetGlobalMtx() const
{
	return globalMatrix;
}

BoneTransform Bone::GetAnimationTransform() const
{
	return animationTransform;
}

u_int Bone::GetIndex() const
{
	return boneIdx;
}
