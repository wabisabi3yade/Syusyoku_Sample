#pragma once
#include "IClassNameGetter.h"

/// @brief アニメーションの通知インターフェース
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public ISaveLoad
{
	/// @brief 活動状態
	bool isActive;

	/// @brief イベントの名前
	std::string notifyName;
public:
	AnimationNotify_Base();
	virtual ~AnimationNotify_Base() {}

	/// @brief アニメーションの通知の更新
	/// @param _lastPlayingRatio アニメーションの前回の再生割合
	/// @param _curPlayingRatio アニメーションの現在の再生割合
	/// @param _isLoop 割合がループしたか？
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	// 活動状態をセット
	void SetIsActive(bool _isActive);

	// イベント名を取得
	void SetNotifyName(const std::string& _notifyName);

	// 活動状態を取得
	bool GetIsActive() const;

	// イベント名取得
	std::string GetNotifyName() const;

	// 型名を取得する
	virtual std::string GetTypeName() const = 0;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

protected:
	void ImGuiSetting() override;
};

namespace HashiTaku
{
	// コンセプトで取得できる
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}

