#pragma once

/// @brief �A�j���[�V�������̒��_�ւ̉e���x
struct Weight
{
	/// @brief �{�[����(�f�o�b�O�p)
	std::string boneName{ "" };

	/// @brief ���b�V����(�f�o�b�O�p)
	std::string meshName{ "" };

	/// @brief ���_�ւ̉e���x(0�`1)
	float weight{ 0.0f };

	/// @brief ���_�C���f�b�N�X
	int	vertexIndex{ 0 };
};

/// @brief ���f����1�{�[�������̃N���X
class Bone
{
	/// @brief �{�[����(�f�o�b�O�p)
	std::string boneName;

	/// @brief ���b�V����(�f�o�b�O�p)
	std::string meshName;

	/// @brief �A�[�}�`���A��(�f�o�b�O�p)
	std::string armatureName;

	/// @brief �R���r�l�[�V�����s��
	DirectX::SimpleMath::Matrix combinationMatrix;

	/// @brief �A�j���[�V�����s��
	DirectX::SimpleMath::Matrix animationMatrix;

	/// @brief �{�[���I�t�Z�b�g�s��
	DirectX::SimpleMath::Matrix offsetMatrix;

	/// @brief �{�[���C���f�b�N�X
	u_int boneIdx;

	/// @brief �E�F�C�g�l
	std::vector<Weight> weights;

public:
	Bone() : boneName(""), meshName(""), armatureName(""), boneIdx(0) {}
	~Bone() {}

	/// @brief �E�F�C�g��z��ɒǉ�����
	/// @param _weight �E�F�C�g
	void AddWeight(const Weight& _weight);

	// ���O�Z�b�g
	void SetBoneName(const std::string& _boneName);
	void SetMeshName(const std::string& _meshName);
	void SetArmatureName(const std::string& _armatureName);

	// �s��Z�b�g
	void SetAnimationMtx(const DirectX::SimpleMath::Matrix& _animationMatrix);
	void SetOffeetMtx(const DirectX::SimpleMath::Matrix& _offsetMatrix);

	// �C���f�b�N�X�Z�b�g
	void SetIndex(u_int _idx);

	// ���O�擾
	std::string GetBoneName() const;
	std::string GetMeshName() const;
	std::string GetArmatureName() const;

	// �s����擾
	DirectX::SimpleMath::Matrix& GetCombMtx();
	DirectX::SimpleMath::Matrix& GetAnimMtx();
	DirectX::SimpleMath::Matrix& GetOffsetMtx();

	void CreateCombMtx(const DirectX::SimpleMath::Matrix& _parentMtx);

	// �C���f�b�N�X���擾
	u_int GetIndex();
};
