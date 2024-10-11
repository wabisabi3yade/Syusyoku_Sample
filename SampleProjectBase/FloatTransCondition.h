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
	/// @brief 比較する値
	float compareVal;

	/// @brief 判別タイプ
	JudgeType judgeType;
public:
	FloatTransCondition(const std::string& _parameterName);
	~FloatTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue)const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:

	void ImGuiSetting() override;
};

