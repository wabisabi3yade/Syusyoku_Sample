#include "pch.h"
#include "Bone.h"

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

u_int Bone::GetIndex()
{
	return boneIdx;
}
