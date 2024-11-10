#pragma once
#include "TransCondition_Base.h"

/// @brief bool型変数を見て、遷移条件達成しているか
class BoolTransCondition : public TransCondition_Base
{
	/// @brief bool型条件を持たすための判定
	enum class JudgeType
	{
		True,
		False
	};

#ifdef EDIT
	/// @brief 判定タイプの文字列
	static std::vector<std::string> judgeTypeStrings;
#endif
private:
	/// @brief 判別タイプ
	JudgeType judgeType;
public:
	BoolTransCondition(const std::string& _parameterName);
	~BoolTransCondition() {}

	bool IsCondition(HashiTaku::AnimParam::conditionValType _checkValue)const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void ImGuiDebug() override;
};

