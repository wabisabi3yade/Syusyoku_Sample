#pragma once
#include "IClassNameGetter.h"

/// @brief アニメーションの通知インターフェース
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public IClassNameGetter
{
	/// @brief 活動状態
	bool isActive;

public:
	AnimationNotify_Base();
	~AnimationNotify_Base() {}

	/// @brief アニメーションの通知の更新
	/// @param _lastPlayingRatio アニメーションの前回の再生割合
	/// @param _curPlayingRatio アニメーションの現在の再生割合
	/// @param _isLoop 割合がループしたか？
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	// 活動状態をセット
	void SetIsActive(bool _isActive);

	// 活動状態を取得
	bool GetIsActive() const;

	virtual std::string ClassNameToStr() override = 0;
protected:
	void ImGuiSetting() override;
};

namespace HashiTaku
{
	// コンセプトで取得できる
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;
}

