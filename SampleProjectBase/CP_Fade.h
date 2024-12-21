#pragma once
#include "Component.h"

namespace HashiTaku
{
	class CP_UIRenderer;

	/// @brief �t�F�[�h�N���X
	class CP_Fade : public Component
	{
		// ���
		enum class State
		{
			Wait, // �������Ȃ�
			Open, // �t�F�[�h�𔖂�����
			Close // �t�F�[�h��Z������
		};

		/// @brief �t�F�[�h�I�����ɋN�����֐�
		std::function<void()> endFunction;

		/// @brief UI�`��N���X
		CP_UIRenderer* pUIRenderer;

		/// @brief �t�F�[�h�̏��
		State state;

		/// @brief �Ăяo����ăt�F�[�h���鎞��
		float settingFadeTime;

		/// @brief �t�F�[�h�ŕς���a�l�̑��x
		float fadeSpeed;

		/// @brief �ړI�Ƃ��郿�l
		float targetAlpha;

		/// @brief �t�F�[�h�ŕ߂鎞��
		float fadeCloseTime;

		/// @brief �t�F�[�h�ŊJ���鎞��
		float fadeOpenTime;

		/// @brief �X�^�[�g���Ƀt�F�[�h���J���ĊJ�n����悤�ɂ���
		bool isStartOpen;
	public:
		CP_Fade();
		~CP_Fade() {}

		/// @brief �t�F�[�h���󂯂鎞��
		void OpenFade();

		/// @brief �t�F�[�h���󂯂鎞��
		/// @param _openTime �J���鎞��
		/// @param _targetAlpha �ڕW�̃�
		void OpenFade(float _openTime, float _targetAlpha = 0.0f);

		/// @brief �t�F�[�h�ŕ߂鎞��
		void CloseFade();

		/// @brief �t�F�[�h��߂鎞��
		/// @param _closeTime �߂鎞��
		/// @param _targetAlpha �ڕW�̃�
		void CloseFade(float _closeTime, float _targetAlpha = 1.0f);

		/// @brief �t�F�[�h�I�����̊֐����Z�b�g����
		/// @param _func �֐�
		void SetOnEndFunction(const std::function<void()>& _func);

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief �J�n����
		void Awake() override;

		/// @brief �X�V����
		void Update() override;

		/// @brief �߂�X�V����
		void CloseUpdate();

		/// @brief �t�F�[�h�̑��x�����߂�
		void CalcFadeSpeed();

		/// @brief �J����X�V
		void OpenUpdate();

		/// @brief �X�V�ł��邩����
		/// @return �X�V�ł��邩�H
		bool GetCanUpdate() const;

		/// @brief ���s����
		void ExecEndFunction();

		void ImGuiDebug() override;
	};
}