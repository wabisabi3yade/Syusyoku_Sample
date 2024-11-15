#pragma once

class IObjectTargeter;

/// @brief �v���C���[�̃^�[�Q�b�g�̕W�I�ɂȂ�C���^�[�t�F�[�X
class ITargetAccepter
{

public:
	ITargetAccepter() {}
	virtual ~ITargetAccepter() {}

	/// @brief ���̃I�u�W�F�N�g�̃��[���h���W���擾
	/// @return ���[���h���W���擾
	virtual DirectX::SimpleMath::Vector3 GetWorldPos() const = 0;

	/// @brief �^�[�Q�b�g���鑤���擾����
	/// @param _targeter �^�[�Q�b�g���鑤
	virtual void GetTargeter(IObjectTargeter& _targeter) = 0;

	/// @brief �^�[�Q�b�g���Ɏ��񂾂��Ƃ�ʒm
	virtual void OnDeathNotifyTargeter() = 0;
};

/// @brief �^�[�Q�b�g����l�̃C���^�[�t�F�[�X
class IObjectTargeter
{

public:
	IObjectTargeter() {}
	virtual ~IObjectTargeter() {}

	/// @brief �^�[�Q�b�g�I�u�W�F�N�g���擾����
	/// @param _targetObject �^�[�Q�b�g�I�u�W�F�N�g
	virtual void GetTargetObject(ITargetAccepter& _targetObject) = 0;

	/// @brief �^�[�Q�b�g�����񂾂Ƃ��̏���
	virtual void OnTargetDeath() = 0;
};

