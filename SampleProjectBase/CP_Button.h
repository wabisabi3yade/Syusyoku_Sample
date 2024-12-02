#pragma once
#include "Component.h"

/// @brief ボタンクラス
class CP_Button : public Component
{
	/// @brief イベント時の関数ラムダ式
	std::function<void()> eventFunction;

public:
	CP_Button() {};
	virtual ~CP_Button() {}

	/// @brief イベント関数
	void OnEvent();

	/// @brief イベント時の関数をセット
	/// @param _setFunction セット
	void SetEventFunction(const std::function<void()>& _setFunction);
};

