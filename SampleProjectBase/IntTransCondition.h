#pragma once
#include "TransCondition_Base.h"

/// @brief intŒ^•Ï”‚ğŒ©‚ÄA‘JˆÚğŒ’B¬‚µ‚Ä‚¢‚é‚©
class IntTransCondition : public TransCondition_Base
{
public:
	/// @brief intŒ^ğŒ‚ğ‚½‚·‚½‚ß‚Ì”»’è
	enum class JudgeType
	{
		Greater,	// ‚æ‚èã
		Less,	// ‚æ‚è‰º
		Equal,	// “™‚µ‚¢
		NotEqual	// “™‚µ‚­‚È‚¢
	};

#ifdef EDIT
	/// @brief ”»’èƒ^ƒCƒv‚Ì•¶š—ñ
	static std::vector<std::string> judgeTypeStrings;
#endif
private:
	/// @brief ‘JˆÚğŒ‚ÅQÆ‚·‚éint•Ï”
	const int* pParameter;

	/// @brief ”äŠr‚·‚é’l
	int compareVal;

	JudgeType judgeType;
public:
	IntTransCondition(const int& _parameter, const std::string& _parameterName);
	~IntTransCondition() {}

	bool IsCondition() const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void ImGuiSetting() override;
};

