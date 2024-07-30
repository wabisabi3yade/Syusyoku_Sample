#pragma once

/// @brief ���f����1�{�[�������̃N���X
class Bone
{
	/// @brief �{�[����(�f�o�b�O�p)
	std::string boneName;

	/// @brief �R���r�l�[�V�����s��
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief �A�j���[�V�����s��
	DirectX::SimpleMath::Matrix animationMatrix;

	/// @brief �{�[���I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief �{�[���C���f�b�N�X
	u_int boneIdx;

public:
	Bone() : boneName(""), boneIdx(0) {}
	~Bone() {}

	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	// ���O�Z�b�g
	void SetBoneName(const std::string& _boneName);

	// �s��Z�b�g
	void SetAnimationMtx(const DirectX::SimpleMath::Matrix& _animationMatrix);
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	// �C���f�b�N�X�Z�b�g
	void SetIndex(u_int _idx);

	// ���O�擾
	std::string GetBoneName() const;

	// �s����擾
	const DirectX::SimpleMath::Matrix& GetCombMtx() const;
	const DirectX::SimpleMath::Matrix& GetAnimMtx() const;
	const DirectX::SimpleMath::Matrix& GetOffsetMtx() const;

	// �C���f�b�N�X���擾
	u_int GetIndex() const;
};
