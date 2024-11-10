#pragma once
#include "TransCondition_Base.h"

/// @brief int型変数を見て、遷移条件達成しているか
class IntTransCondition : public TransCondition_Base
{
public:
	/// @brief int型条件を持たすための判定
	enum class JudgeType
	{
		Greater,	// より上
		Less,	// より下
		Equal,	// 等しい
		NotEqual	// 等しくない
	};

#ifdef EDIT
	/// @brief 判定タイプの文字列
	static std::vector<std::string> judgeTypeStrings;
#endif
private:
	/// @brief 比較する値
	int compareVal;

	JudgeType judgeType;
public:
	IntTransCondition(const std::string& _parameterName);
	~IntTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue) const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void ImGuiDebug() override;
};

