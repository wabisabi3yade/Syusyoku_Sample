#pragma once

// �{�[���̃g�����X�t�H�[��
struct BoneTransform
{
	DirectX::SimpleMath::Vector3 position;
	DirectX::SimpleMath::Vector3 scale{ 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Quaternion rotation;
};

/// @brief ���f����1�{�[�������̃N���X
class Bone
{
	/// @brief �{�[����(�f�o�b�O�p)
	std::string boneName;

	/// @brief �R���r�l�[�V�����s��
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief �{�[���I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief �{�[���̎p���s��
	DirectX::SimpleMath::Matrix globalMatrix;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[��
	BoneTransform animationTransform;

	/// @brief �{�[���C���f�b�N�X
	u_int boneIdx;

public:
	Bone() : boneName(""), boneIdx(0) {}
	Bone(const Bone& _other);
	~Bone() {}
	Bone& operator=(const Bone& _other);

	/// @brief �R���r�l�[�V�����s����쐬
	/// @param _parentMtx �e�m�[�h�܂ł̃��[�J���g�����X�t�H�[��
	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	/// @brief ���[�J����Ԃł̃{�[�����W
	/// @param _parentMtx �e�m�[�h�܂ł̃��[�J���g�����X�t�H�[��
	/// @param _offsetMtx �I�t�Z�b�g�s��
	void CreateGlobalMtx(const DirectX::SimpleMath::Matrix& _parentMtx, const DirectX::SimpleMath::Matrix& _offsetMtx);

	// ���O�Z�b�g
	void SetBoneName(const std::string& _boneName);

	// �s��Z�b�g
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	/// @brief �{�[���̃A�j���[�V�����g�����X�t�H�[���̎Q�Ƃ��擾
	/// @return �A�j���[�V�����g�����X�t�H�[���̎Q��
	BoneTransform& GetRefelenceAnimTransform();

	// �C���f�b�N�X�Z�b�g
	void SetIndex(u_int _idx);

	// ���O�擾
	std::string GetBoneName() const;

	// �s����擾
	const DirectX::SimpleMath::Matrix& GetCombMtx() const;
	DirectX::SimpleMath::Matrix GetAnimMtx() const;
	const DirectX::SimpleMath::Matrix& GetOffsetMtx() const;
	const DirectX::SimpleMath::Matrix& GetGlobalMtx() const;

	// �g�����X�t�H�[�����擾
	BoneTransform GetAnimationTransform() const;

	// �C���f�b�N�X���擾
	u_int GetIndex() const;

private:
	void Copy(const Bone& _other);
};
