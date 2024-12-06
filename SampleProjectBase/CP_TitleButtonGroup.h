#pragma once
#include "CP_ButtonGroup.h"

/// @brief タイトルのボタン管理
class CP_TitleButtonGroup : public CP_ButtonGroup
{
	/// @brief タイトルで使用するボタンの種類
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

	/// @brief ボタンの初期化
	void ButtonSetup();
};

