#pragma once
#include "AnimationNotifyEvent.h"
#include "IAnimParametersSetter.h"

class ANE_ChangeParameter : public AnimationNotifyEvent, public IAnimParametersSetter
{
	/// @brief �ύX�������p�����[�^��
	std::string changeParamName;

	/// @brief �A�j���[�V�������p�����[�^
	AnimationParameters* pAnimationParameters;

	/// @brief �C�x���g�ő������l
	bool setParam;
public:
	ANE_ChangeParameter();
	~ANE_ChangeParameter() {}

	/// @brief �A�j���[�V�����p�����[�^�Z�b�g
	/// @param _animatiionParameters �A�j���[�V�����p�����[�^
	void SetAnimationParameters(AnimationParameters& _animatiionParameters) override;

	/// @brief �N���[���֐�
	/// @return ���������ʒm�C�x���g
	std::unique_ptr<AnimationNotify_Base> Clone() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	/// @brief �C�x���g�N��������
	void OnEvent() override;

	/// @brief  ������
	void OnInitialize() override;

	/// @brief �I��
	void OnTerminal() override;

	void ImGuiDebug() override;
};

