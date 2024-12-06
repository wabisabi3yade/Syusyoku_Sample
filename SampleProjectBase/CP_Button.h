#pragma once
#include "Component.h"

/// @brief �{�^���N���X
class CP_Button : public Component
{
	/// @brief �C�x���g���̊֐������_��
	std::function<void()> eventFunction;

public:
	CP_Button() {};
	virtual ~CP_Button() {}

	/// @brief �C�x���g�֐�
	void OnEvent();

	/// @brief �C�x���g���̊֐����Z�b�g
	/// @param _setFunction �Z�b�g
	void SetEventFunction(const std::function<void()>& _setFunction);
};

