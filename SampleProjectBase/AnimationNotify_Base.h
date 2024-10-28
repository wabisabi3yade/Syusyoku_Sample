#pragma once

class GameObject;

/// @brief アニメーションの通知インターフェース
class AnimationNotify_Base : public HashiTaku::IImGuiUser, public  HashiTaku::ISaveLoad
{
public:
	/// @brief 通知イベント種類
	enum class NotifyType
	{
		// デバッグログ
		ANE_DebugLog,
		ANS_DebugLog,

		// パラメータ変更
		ANE_ChangeParameter,
		ANS_ChangeParameter,

		None
	};

private:
	/// @brief 活動状態
	bool isActive;

	/// @brief イベントの名前
	std::string notifyName;

	/// @brief イベント種類
	NotifyType notifyType;
#ifdef EDIT
	bool isEditing{ false };
#endif // EDIT

public:
	AnimationNotify_Base();
	AnimationNotify_Base(NotifyType _notifyType);
	virtual ~AnimationNotify_Base() {}

	void Init();

	/// @brief アニメーションの通知の更新
	/// @param _lastPlayingRatio アニメーションの前回の再生割合
	/// @param _curPlayingRatio アニメーションの現在の再生割合
	/// @param _isLoop 割合がループしたか？
	virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, bool _isLoop) = 0;

	/// @brief 次の通知イベントに切り替える前の終了処理
	virtual void OnTerminal() {};

	// 活動状態をセット
	void SetIsActive(bool _isActive);

	// イベント名を取得
	void SetNotifyName(const std::string& _notifyName);

	/// @brief 種類から名前に変換
	/// @param _notifyType 種類
	/// @return 名前
	static std::string GetTypeToStr(NotifyType _notifyType);

	// 活動状態を取得
	bool GetIsActive() const;

	// イベント名取得
	std::string GetNotifyName() const;

	/// @brief 通知イベントの種類を取得する
	/// @return 通知イベントの種類
	NotifyType GetNotifyType() const;

	/// @brief クローン関数
	/// @return 生成した通知イベント
	virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

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

