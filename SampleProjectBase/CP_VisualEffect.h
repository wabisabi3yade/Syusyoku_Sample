#pragma once
#include "Component.h"
#include "VisualEffect.h"
#include "DX11EffecseerManager.h"

namespace HashiTaku
{
	/// @brief �G�t�F�N�g���Đ�����R���|�[�l���g
	class CP_VisualEffect : public Component
	{
		/// @brief DirectX�̃G�t�F�N�g�}�l�W���[
		DX11EffecseerManager* pDxVfxManager;

		/// @brief �Đ�����G�t�F�N�g
		const VisualEffect* pVFX;

		/// @brief �Đ�����G�t�F�N�g�̃n���h��
		Effekseer::Handle vfxHandle;

		/// @brief �Đ����x
		float playSpeed;

		/// @brief ���݂̃t���[��
		float currentPlayFrame;

		/// @brief �Đ��J�n�t���[��
		int playStartFrame;

		/// @brief �Đ��I���t���[��
		int playEndFrame;

		/// @brief �Đ������H
		bool isPlaying;

		/// @brief ���[�v�Đ����邩�H
		bool isLoop;

		/// @brief �Đ����Ԃ͈͎̔w�肷�邩�H
		bool isTrimming;
	public:
		CP_VisualEffect();
		~CP_VisualEffect() {}

		void Init() override;

		/// @brief �Đ����x��ύX
		/// @param _playSpeed �Đ����x
		void SetPlaySpeed(float _playSpeed);

		/// @brief �G�t�F�N�g���Z�b�g
		/// @param _setVfx �G�t�F�N�g
		void SetVisualEffect(const VisualEffect* _setVfx);

		/// @brief �Z�[�u����
		/// @param _data �Z�[�u�V�[���f�[�^
		json Save() override;

		/// @brief ���[�h����
		/// @param _data ���[�h����V�[���f�[�^ 
		void Load(const json& _data) override;
	private:
		void Start() override;

		void Update() override;

		/// @brief �g���~���O�֘A�̍X�V����
		void TrimmingUpdate();

		/// @brief �I���������̏���
		void PlayEndUpdate();

		/// @brief �G�t�F�N�g���Đ��J�n����
		void BeginPlayVFX();

		/// @brief �G�t�F�N�g�Đ���~����
		void StopPlayVFX();

		void ImGuiDebug() override;
	};
}