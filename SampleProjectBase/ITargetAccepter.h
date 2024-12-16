#pragma once

namespace HashiTaku
{
	class IObjectTargeter;

	/// @brief �v���C���[�̃^�[�Q�b�g�̕W�I�ɂȂ�C���^�[�t�F�[�X
	class ITargetAccepter
	{

	public:
		ITargetAccepter() {}
		virtual ~ITargetAccepter() {}

		/// @brief ���̃I�u�W�F�N�g�̃��[���h���W���擾
		/// @return ���[���h���W���擾
		virtual const DirectX::SimpleMath::Vector3& GetWorldPos() const = 0;

		/// @brief �^�[�Q�b�g���鑤���擾����
		/// @param _targeter �^�[�Q�b�g���鑤
		virtual void SetTargeter(IObjectTargeter& _targeter) = 0;

		/// @brief �^�[�Q�b�g���鑤���폜����
		/// @param _targeter �폜�^�[�Q�b�g���鑤
		virtual void RemoveTargeter(IObjectTargeter& _targeter) = 0;

		/// @brief �^�[�Q�b�g���ɉ������邱�Ƃ�ʒm
		virtual void OnRemoveNotifyToTargeter() = 0;
	};

	/// @brief �^�[�Q�b�g����l�̃C���^�[�t�F�[�X
	class IObjectTargeter
	{

	public:
		IObjectTargeter() {}
		virtual ~IObjectTargeter() {}

		/// @brief �^�[�Q�b�g�I�u�W�F�N�g���擾����
		/// @param _targetObject �^�[�Q�b�g�I�u�W�F�N�g
		virtual void SetTargetObject(ITargetAccepter& _targetObject) = 0;

		/// @brief ��������Ƃ��̍X�V����
		/// @param _removeObj ���񂾃^�[�Q�b�g�I�u�W�F�N�g
		virtual void RemoveNotify(const ITargetAccepter& _removeObj) = 0;
	};
}