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

	void Bone::CreateCombMtx(const DXSimp::Matrix& _parentMtx)
	{
		combinationMatrix = offsetMatrix * GetAnimMtx() * _parentMtx;
	}

	void Bone::CreateGlobalMtx(const DXSimp::Matrix& _parentMtx, const DXSimp::Matrix& _offsetMtx)
	{
		globalMatrix = GetAnimMtx() * _parentMtx * _offsetMtx;
	}

	void Bone::SetBoneName(const std::string& _boneName)
	{
		boneName = _boneName;
	}

	void Bone::SetOffeetMtx(const DXSimp::Matrix& _offsetMatrix)
	{
		offsetMatrix = _offsetMatrix;
	}

	void Bone::SetLocalNodeMtx(const DXSimp::Matrix& _nodeMatrix)
	{
		// ツリーノードからの初期行列をトランスフォームに変換し、代入
		Mtx::GetTransformFromWldMtx(
			_nodeMatrix,
			localNodeTransform.position,
			localNodeTransform.scale,
			localNodeTransform.rotation
			);
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

	const DXSimp::Matrix& Bone::GetCombMtx() const
	{
		return combinationMatrix;
	}

	DXSimp::Matrix Bone::GetAnimMtx() const
	{
		// アニメーション行列を作成
		Matrix scaleMtx = Matrix::CreateScale(animationTransform.scale);
		Matrix rotationMtx = Matrix::CreateFromQuaternion(animationTransform.rotation);
		Matrix transformMtx = Matrix::CreateTranslation(animationTransform.position);
		Matrix animationMtx = scaleMtx * rotationMtx * transformMtx;

		return animationMtx;
	}

	const DXSimp::Matrix& Bone::GetOffsetMtx() const
	{
		return offsetMatrix;
	}

	const DXSimp::Matrix& Bone::GetGlobalMtx() const
	{
		return globalMatrix;
	}

	const BoneTransform& Bone::GetAnimationTransform() const
	{
		return animationTransform;
	}

	const BoneTransform& Bone::GetLocalNodeTransform() const
	{
		return localNodeTransform;
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
		localNodeTransform = _other.localNodeTransform;
		boneIdx = _other.boneIdx;
	}
}