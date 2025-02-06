#pragma once

namespace HashiTaku
{
	class CP_Animation;
	class CP_Character;

	/// @brief �A�N�V�����R���g���[���[�̃C���^�[�t�F�[�X
	class IActionController
	{

	public:
		/// @brief �w�肵���x�N�g���̗͂�����
		/// @param _power ��
		virtual void AddForce(const DXSimp::Vector3& _power) = 0;

		/// @brief �w�肵���x�N�g���̗͂��Ռ��Ƃ��ē����
		/// @param _power ��
		virtual void AddImpulse(const DXSimp::Vector3& _power) = 0;

		/// @brief ���x���Z�b�g
		/// @param _velocity ���x
		virtual void SetVelocity(const DXSimp::Vector3& _velocity) = 0;

		/// @brief �w�肵��bool�ϐ��ɒl���Z�b�g
		/// @param _paramName �p�����[�^�[��
		/// @param _isBool �Z�b�g����l
		virtual void SetAnimationBool(const std::string& _paramName, bool _isBool) = 0;

		/// @brief �w�肵��int�ϐ��ɒl���Z�b�g
		/// @param _paramName �p�����[�^�[��
		/// @param _intVall �Z�b�g����l
		virtual void SetAnimationInt(const std::string& _paramName, int _intVal) = 0;

		/// @brief �w�肵��float�ϐ��ɒl���Z�b�g
		/// @param _paramName �p�����[�^�[��
		/// @param _floatVal �Z�b�g����l
		virtual void SetAnimationFloat(const std::string& _paramName, float _floatVal) = 0;

		/// @brief �w�肵���g���K�[�ϐ���true�ɂ���
		/// @param _paramName �p�����[�^�[��
		virtual void SetAnimationTrigger(const std::string& _paramName) = 0;

		/// @brief �L�����N�^�[���擾����
		/// @return �L�����N�^�[�R���|�[�l���g
		virtual CP_Character& GetCharacter() = 0;

		/// @brief ���g�̃I�u�W�F�N�g�̃g�����X�t�H�[��
		/// @return �I�u�W�F�N�g�̃g�����X�t�H�[��
		virtual Transform& GetMyTransform() = 0;

		/// @brief �A�j���[�V�����R���|�[�l���g�擾
		/// @return �A�j���[�V�����R���|�[�l���g
		virtual CP_Animation* GetAnimation() = 0;

		/// @brief ���x���擾
		/// @param _outVelocity �擾�p�ϐ�
		virtual void GetVelocity(DXSimp::Vector3& _outVelocity) = 0;

		/// @brief ���x���擾
		/// @return ���x
		virtual DXSimp::Vector3 GetVelocity() const = 0;

		/// @brief �w�肵��bool�ϐ��ɒl���擾
		/// @param _paramName �p�����[�^�[��
		/// @param _isBool �擾����l
		virtual bool GetAnimationBool(const std::string& _paramName) = 0;

		/// @brief �w�肵��int�ϐ��ɒl���擾
		/// @param _paramName �p�����[�^�[��
		/// @param _intVall �擾����l
		virtual int GetAnimationInt(const std::string& _paramName) = 0;

		/// @brief �w�肵��float�ϐ��ɒl���擾
		/// @param _paramName �p�����[�^�[��
		/// @param _floatVal �擾����l
		virtual float GetAnimationFloat(const std::string& _paramName) = 0;

		/// @brief ��t���擾����
		/// @return ��t
		virtual float DeltaTime() const = 0;
	};
}


