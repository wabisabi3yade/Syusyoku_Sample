#pragma once

/// @brief 変数を参照し、遷移条件を変更したり達成しているか確認するクラス
class TransCondition_Base : HashiTaku::IImGuiUser
{
public:
	TransCondition_Base() {}
	virtual ~TransCondition_Base() {}

	virtual bool IsCondition() const = 0;
protected:
	virtual void ImGuiSetting() override = 0;
};

