#include "pch.h"
#include "Bone.h"

using namespace DirectX::SimpleMath;

void Bone::CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx)
{
	combinationMatrix = offsetMatrix * animationMatrix * _parentMtx ;
}

void Bone::SetBoneName(const std::string& _boneName)
{
	boneName = _boneName;
}

void Bone::SetAnimationMtx(const DirectX::SimpleMath::Matrix& _animationMatrix)
{
	animationMatrix = _animationMatrix;
}

void Bone::SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix)
{
	offsetMatrix = _offsetMatrix;
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
 
const DirectX::SimpleMath::Matrix& Bone::GetAnimMtx() const
{
	return animationMatrix;
}

const DirectX::SimpleMath::Matrix& Bone::GetOffsetMtx() const
{
	return offsetMatrix;
}

u_int Bone::GetIndex() const
{
	return boneIdx;
}
