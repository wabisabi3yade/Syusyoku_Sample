#pragma once
#include "AnimationNotifyState.h"
#include "IAnimParametersSetter.h"
#include "AnimationParameterType.h"

/// @brief �A�j���[�V�����ʒm�Ńp�����[�^��ύX����(bool�^�����ύX����\��Ȃ��̂�bool�Ή�)
class ANS_ChangeParameter : public AnimationNotifyState, public IAnimParametersSetter
{
	//�Z�N�V�����̎��
	enum class SectionType
	{
		Init,	// ����
		Tick,	// �X�V���̒l
		End,	// �I��
		Terminal, // �ύX�O�I������
		Num
	};

	/// @brief �Z�N�V������
	static constexpr short SECTION_CNT = static_cast<short>(SectionType::Num);

	/// @brief �e�Z�N�V�����Ŏg�p����Ă��邩�H
	std::array<bool, SECTION_CNT> isUsing;

	/// @brief �e�Z�N�V�����ŃZ�b�g����p�����[�^
	std::array<bool, SECTION_CNT> setParameter;

	/// @brief �ύX�������p�����[�^��
	std::string changeParamName;

	/// @brief �A�j���[�V�������p�����[�^
	AnimationParameters* pAnimationParameters;

	/// @brief ���݌��Ă���p�����[�^�̌^
	HashiTaku::AnimParam::TypeKind parameterType;
public:
	ANS_ChangeParameter();
	~ANS_ChangeParameter() {};

	/// @brief �A�j���[�V�����p�����[�^���X�g�Z�b�g
	/// @param _animatiionParameters �A�j���[�V�������X�g�p�����[�^
	void SetAnimationParameters(AnimationParameters& _animatiionParameters) override;

	/// @brief �p�����[�^��ύX
	/// @param _parameterName �ύX�������p�����[�^��
	void ChangeAnimationParameter(const std::string& _parameterName);

	/// @brief �N���[���֐�
	/// @return ���������ʒm�C�x���g
	std::unique_ptr<AnimationNotify_Base> Clone() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �C�x���g�Ԃ̍X�V����
	void Begin() override {};

	/// @brief �C�x���g�Ԃ̍X�V����
	void Tick() override;

	/// @brief �C�x���g�I������
	void End() override;

	/// @brief �؂�ւ��͂��߂̏���
	void OnInitialize() override;

	/// @brief �؂�ւ��O�I������
	void OnTerminal() override;

	/// @brief �p�����[�^�Z�b�g�ł��邩
	/// @param _type �Z�N�V����
	/// @return �ł��邩�H
	bool IsCanSet(SectionType _type);

	/// @brief �Z�N�V������short�ɕϊ�
	/// @param _type �Z�N�V����
	/// @return short
	short CastShort(SectionType _type);

	void ImGuiDebug() override;
};

