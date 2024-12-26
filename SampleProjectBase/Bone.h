#pragma once

namespace HashiTaku
{
	/// @brief  �{�[���̃g�����X�t�H�[��
	struct BoneTransform
	{
		DXSimp::Vector3 position;
		DXSimp::Vector3 scale{ 1.0f, 1.0f, 1.0f };
		DXSimp::Quaternion rotation;
	};

	/// @brief ���f����1�{�[�������̃N���X
	class Bone
	{
		/// @brief �{�[����(�f�o�b�O�p)
		std::string boneName;

		/// @brief �R���r�l�[�V�����s��
		DXSimp::Matrix combinationMatrix;

		/// @brief �{�[���I�t�Z�b�g�s��
		DXSimp::Matrix offsetMatrix;

		/// @brief �{�[���̎p���s��
		DXSimp::Matrix globalMatrix;

		/// @brief �A�j���[�V�����̃g�����X�t�H�[��
		BoneTransform animationTransform;

		/// @brief �e�{�[������̏����p���g�����X�t�H�[��
		BoneTransform localNodeTransform;

		/// @brief �{�[���C���f�b�N�X
		u_int boneIdx;
	public:
		Bone() : boneName(""), boneIdx(0) {}
		Bone(const Bone& _other);
		~Bone() {}
		Bone& operator=(const Bone& _other);

		/// @brief �R���r�l�[�V�����s����쐬
		/// @param _parentMtx �e�m�[�h�܂ł̃��[�J���g�����X�t�H�[��
		void CreateCombMtx(const DXSimp::Matrix& _parentMtx);

		/// @brief ���[�J����Ԃł̃{�[�����W
		/// @param _parentMtx �e�m�[�h�܂ł̃��[�J���g�����X�t�H�[��
		/// @param _offsetMtx �I�t�Z�b�g�s��
		void CreateGlobalMtx(const DXSimp::Matrix& _parentMtx, const DXSimp::Matrix& _offsetMtx);

		// ���O�Z�b�g
		void SetBoneName(const std::string& _boneName);

		// �s��Z�b�g
		void SetOffeetMtx(const DXSimp::Matrix& _offsetMatrix);

		/// @brief ���[�J���m�[�h�s����Z�b�g
		/// @param _nodeMatrix �m�[�h�s��
		void SetLocalNodeMtx(const DXSimp::Matrix& _nodeMatrix);

		/// @brief �{�[���̃A�j���[�V�����g�����X�t�H�[���̎Q�Ƃ��擾
		/// @return �A�j���[�V�����g�����X�t�H�[���̎Q��
		BoneTransform& GetRefelenceAnimTransform();

		// �C���f�b�N�X�Z�b�g
		void SetIndex(u_int _idx);

		// ���O�擾
		std::string GetBoneName() const;

		// �s����擾
		const DXSimp::Matrix& GetCombMtx() const;
		DXSimp::Matrix GetAnimMtx() const;
		const DXSimp::Matrix& GetOffsetMtx() const;
		const DXSimp::Matrix& GetGlobalMtx() const;

		// �g�����X�t�H�[�����擾
		const BoneTransform& GetAnimationTransform() const;

		/// @brief �����p�����[�J���g�����X�t�H�[�����擾
		/// @return �����p�����[�J���g�����X�t�H�[��
		const BoneTransform& GetLocalNodeTransform() const;

		// �C���f�b�N�X���擾
		u_int GetIndex() const;

	private:
		void Copy(const Bone& _other);
	};
}