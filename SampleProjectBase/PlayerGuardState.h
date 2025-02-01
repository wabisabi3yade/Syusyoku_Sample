#pragma once
#include "PlayerGroundState.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	class CP_CameraMove;

	class PlayerGuardState : public PlayerGroundState
	{
		/// @brief �p���B���̃G�t�F�N�g���
		CreateVfxInfo parryEffectInfo;

		/// @brief �p���B���̃J������h�炷�p�����[�^�[
		PerlinShakeParameter parryCamShakeParam;

		/// @brief �p���B�̌��ʉ�
		std::list<PlaySoundParameter> parrySoundParameters;

		/// @brief �p���B�G�t�F�N�g�𐶐�����ꏊ�̃I�t�Z�b�g(�I�u�W�F�N�g����)
		DXSimp::Vector3 createVfxOffset;

		/// @brief �J�����ړ��N���X
		CP_CameraMove* pCameraMove;

		/// @brief �p���B�ł����Ԃ̎�������W
		u_int sustainParryFrame;

		/// @brief �o�ߎ���
		u_int parryElapsedFrame;

		/// @brief �p���B�ő�����Q�[�W
		float parryAddGuardGage;

		/// @brief �p���B���邱�Ƃ��ł��鐳�ʂ̊p�x
		float canParryForwardAngle;

		/// @brief �p���B�������ɑ��₷�X�^�C���b�V���|�C���g
		float addStylishPointOnParry;

		/// @brief �p���B���p�b�h�U����
		float parryPadVibePower;

		/// @brief �p���B���p�b�h�U������
		float parryPadVibeTime;

		/// @brief �p���B�ł�����
		bool canParry;

		/// @brief �p���B���J��o���t���[�����H
		bool isPerformParryFrame;
	public:
		PlayerGuardState();
		~PlayerGuardState() {}

		/// @brief �p���B�ł��邩�m�F
		/// @return �p���B�ł��邩�H
		bool GetCanParry(const DXSimp::Vector3& _enemyPos);

		/// @brief �p���B�����������s���悤�ɂ���
		void SetPerfomParry();

		/// @brief �Z�[�u����
		/// @return �Z�[�u�f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����f�[�^ 
		void Load(const json& _data) override;
	private:
		/// @brief �J�n
		void OnStartBehavior() override;

		/// @brief �X�V
		void UpdateBehavior() override;

		/// @brief �I��
		void OnEndBehavior() override;

		/// @brief �A�j���[�V�����I�����̍s��
		/// @param _fromAnimNodeName �J�ڌ��̃A�j���[�V�����m�[�h��
		/// @param _toAnimNodeName �J�ڐ�̃A�j���[�V�����m�[�h��
		void OnAnimationEnd(const std::string& _fromAnimNodeName,
			const std::string& _toAnimNodeName) override;

		/// @brief �p���B���̍s��
		void OnParry();

		/// @brief �p���B�ł��鎞�Ԃ̍X�V
		void ParryTimeUpdate();

		/// @brief �p���B���J��o�����`�F�b�N����
		void CheckPerformParry();

		/// @brief �Q�[�W������ċ��͂Ȉꌂ
		void ReleaseAttack();

		/// @brief �p���B�G�t�F�N�g�𐶐�
		void CreateParryVfx();

		/// @brief �J�����V�F�C�N���s��
		void CameraShakeOnParry();

		/// @brief �R���g���[���[��U��
		void PadVibrationOnParry();

		/// @brief ���ʉ���炷
		void PlayParrySE();

		void ImGuiDebug() override;
	private:
		// �K�[�h���̃p�����[�^��
		static constexpr auto GUARD_PARAMNAME{ "isGuard" };
	};
}