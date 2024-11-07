#pragma once
#include "StateMachine.h"

/// @brief キャラクタのアクションステート基底
class CharacterActState_Base : public HashiTaku::StateNode_Base, public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	CharacterActState_Base() {}
	virtual ~CharacterActState_Base() {}

	/// @brief アニメーション終了時の行動
	/// @param _fromAnimNodeName 遷移元のアニメーションノード名
	/// @param _toAnimNodeName 遷移先のアニメーションノード名
	virtual void OnAnimationEnd(const std::string& _fromAnimNodeName, const std::string& _toAnimNodeName)
	{}

	virtual nlohmann::json Save()  = 0;
	virtual void Load(const nlohmann::json& _data) = 0;
protected:
	// 各ステートの振る舞い
	virtual void OnStartBehavior() {}
	virtual void UpdateBehavior() {}
	virtual void OnEndBehavior() {}

	virtual void ImGuiSetting() = 0;
};

