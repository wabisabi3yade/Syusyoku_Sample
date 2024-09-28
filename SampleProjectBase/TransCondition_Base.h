#pragma once
#include "AnimationParameterType.h"

/// @brief 変数を参照し、遷移条件を変更したり達成しているか確認するクラス
class TransCondition_Base : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief 参照している変数の型名
	HashiTaku::AnimParam::TypeKind typeKind;

protected:
	/// @brief パラメータ名
	const std::string* pParameterName;

public:
	TransCondition_Base(const std::string& _parameterName, HashiTaku::AnimParam::TypeKind _typeKind);
	virtual ~TransCondition_Base() {}

	/// @brief 遷移条件を満たしているか確認
	/// @return 遷移条件満たしているか？
	virtual bool IsCondition() const = 0;

	/// @brief 参照しているパラメータ名を取得する
	/// @return 参照しているパラメータ名
	const std::string& GetReferenceParamName() const;

	/// @brief 対応している型の種類を取得する
	/// @return 型の種類
	HashiTaku::AnimParam::TypeKind GetTypeKind() const;

	virtual nlohmann::json Save() = 0;
	virtual void Load(const nlohmann::json& _data) = 0;
protected:
	void ImGuiSetting() override;
};

