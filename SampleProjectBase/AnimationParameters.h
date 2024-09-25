#pragma once
#include "AnimationParameterType.h"

/// @brief アニメーションコントローラー内で使用するパラメータリスト
class AnimationParameters : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
	/// @brief パラメータリスト
	std::unordered_map<std::string, conditionValType> animParameters;
public:
	AnimationParameters() {}
	~AnimationParameters() {}

	template<typename T> requires std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float>
	void AddParameter(std::string _paramNames = "new Parameter");

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief コントローラー内でパラメータ名が被らないようにする
	/// @param _nodename 変更したいパラメータ名
	void NotDuplicateParamName(std::string& _paramName);

	void ImGuiSetting() override;
	// ImGuiでパラメータを新規に追加する
	void ImGuiAddParam();
	// ImGuiでパラメータを表示
	void ImGuiDisplay();
};


template<typename T> requires std::same_as<T, bool> || std::same_as<T, int> || std::same_as<T, float>
inline void AnimationParameters::AddParameter(std::string _paramNames)
{
	// 名前を複しないようにする
	NotDuplicateParamName(_paramNames);

	conditionValType addParam;

	if constexpr (std::is_same<T, bool>::value)
		addParam = false;

	else if constexpr(std::is_same<T, int>::value)
		addParam = 0;

	else if constexpr(std::is_same<T, float>::value)
		addParam = 0.0f;

	animParameters[_paramNames] = addParam;
}
