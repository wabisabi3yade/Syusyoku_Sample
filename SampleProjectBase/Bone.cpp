#include "pch.h"
#include "Bone.h"

namespace HashiTaku
{
	using namespace DXSimp;

	Bone::Bone(const Bone& _other)
	{
		Copy(_other);
	}

	Bone& Bone::operator=(const Bone& _other)
	{
		Copy(_other);

		return *this;
	}

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

	BoneTransform& Bone::GetRefelenceAnimTransform()
	{
		return animationTransform;
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

	void Bone::Copy(const Bone& _other)
	{
		if (this == &_other) return;

		boneName = _other.boneName;
		offsetMatrix = _other.offsetMatrix;
		boneIdx = _other.boneIdx;
	}
}