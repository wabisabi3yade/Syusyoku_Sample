#pragma once

/// @brief �ϐ����Q�Ƃ��A�J�ڏ�����ύX������B�����Ă��邩�m�F����N���X
class TransCondition_Base : HashiTaku::IImGuiUser
{
public:
	TransCondition_Base() {}
	virtual ~TransCondition_Base() {}

	virtual bool IsCondition() const = 0;
protected:
	virtual void ImGuiSetting() override = 0;
};

