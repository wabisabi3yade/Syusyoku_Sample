#pragma once
#include "TransCondition_Base.h"

/// @brief float型変数を見て、遷移条件達成しているか
class FloatTransCondition : public TransCondition_Base
{
public:
	/// @brief float型条件を持たすための判定
	enum class JudgeType
	{
		Greater,	// より上
		Less,	// より下
		Equal	// 等しい
	};

#ifdef EDIT
	/// @brief 判定タイプの文字列
	static std::vector<std::string> judgeTypeStrings;
#endif // EDIT
private:
	/// @brief 遷移条件で参照するfloat変数
	float* pParameter;

	/// @brief 比較する値
	float compareVal;

	/// @brief 判別タイプ
	JudgeType judgeType;
public:
	FloatTransCondition(float& _parameter);
	~FloatTransCondition() {}

	bool IsCondition()const override;
private:

	void ImGuiSetting() override;
};

