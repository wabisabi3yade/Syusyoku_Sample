#include "pch.h"
#include "Bone.h"

using namespace DirectX::SimpleMath;

void Bone::AddWeight(const Weight& _weight)
{
	weights.push_back(_weight);
}

void Bone::SetBoneName(const std::string& _boneName)
{
	boneName = _boneName;
}

void Bone::SetMeshName(const std::string& _meshName)
{
	meshName = _meshName;
}

void Bone::SetArmatureName(const std::string& _armatureName)
{
	armatureName = _armatureName;
}

void Bone::SetCombinationMtx(const DirectX::SimpleMath::Matrix& _combinationMatrix)
{
	combinationMatrix = _combinationMatrix;
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

std::string Bone::GetMeshName() const
{
	return meshName;
}

std::string Bone::GetArmatureName() const
{
	return armatureName;
}

DirectX::SimpleMath::Matrix& Bone::GetCombMtx()
{
	return combinationMatrix;
}

DirectX::SimpleMath::Matrix& Bone::GetAnimMtx()
{
	return animationMatrix;
}

DirectX::SimpleMath::Matrix& Bone::GetOffsetMtx()
{
	return offsetMatrix;
}

void Bone::CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx)
{
	Matrix inverseMtx = offsetMatrix;
	inverseMtx.Invert();

	combinationMatrix = offsetMatrix * animationMatrix * inverseMtx * _parentMtx;
}

u_int Bone::GetIndex()
{
	return boneIdx;
}
