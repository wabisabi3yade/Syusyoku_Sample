#pragma once
#include "CP_ButtonGroup.h"

/// @brief �^�C�g���̃{�^���Ǘ�
class CP_TitleButtonGroup : public CP_ButtonGroup
{
	/// @brief �^�C�g���Ŏg�p����{�^���̎��
	enum class ButtonType
	{
		Start,
		End,
		MaxNum
	};
public:
	CP_TitleButtonGroup();
	~CP_TitleButtonGroup() {}

private:
	void Start() override;

	/// @brief �{�^���̏�����
	void ButtonSetup();
};

