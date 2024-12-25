#pragma once

namespace HashiTaku
{
	class GameObject;

	/// @brief アニメーションの通知インターフェース
	class AnimationNotify_Base : public IImGuiUser, public ISaveLoad
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

			// エフェクト作成
			ANE_CreateVfx,

			// カメラシェイク
			ANE_CameraShake,

			// パッド振動
			ANE_PadShake,

			None
		};

	private:
		/// @brief 活動状態
		bool isActive;

		/// @brief イベントの名前
		std::string notifyName;

		/// @brief イベント種類
		NotifyType notifyType;

		/// @brief アニメーションのフレーム数
		u_int animFrameCnt;
#ifdef EDIT
		bool isEditing{ false };
#endif // EDIT

	public:
		AnimationNotify_Base();

		/// @brief コンストラクタ
		/// @param _notifyType 通知イベント種類
		AnimationNotify_Base(NotifyType _notifyType);
		virtual ~AnimationNotify_Base() {}

		void Init();

		/// @brief アニメーションの通知の更新
		/// @param _lastPlayingRatio アニメーションの前回の再生割合
		/// @param _curPlayingRatio アニメーションの現在の再生割合
		/// @param _isLoop 割合がループしたか？
		virtual void Update(const float _lastPlayingRatio, const float _curPlayingRatio, const bool _isLoop) = 0;

		/// @brief 通知イベントが始まったときの初期化
		void OnInitCall();

		/// @brief  次のアニメーションに切り替える前の終了処理呼び出し関数
		void OnTerminalCall();

		// 活動状態をセット
		void SetIsActive(bool _isActive);

		// イベント名を取得
		void SetNotifyName(const std::string& _notifyName);

		/// @brief アニメーションのフレーム数をセット
		void SetAnimationFrameCnt(u_int _animFrame);

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

		/// @brief 全体のフレーム数を取得
		/// @return フレーム数
		u_int GetAnimFrameCnt() const;

		/// @brief クローン関数
		/// @return 生成した通知イベント
		virtual std::unique_ptr<AnimationNotify_Base> Clone() = 0;

		json Save() override;
		void Load(const json& _data) override;

	protected:
		/// @brief 始まったときの開始処理
		virtual void OnInitialize() {};

		/// @brief 次のアニメーションに切り替える前の終了処理
		virtual void OnTerminal() {};

		/// @brief フレーム数から割合に変換
		/// @param _frame フレーム数
		/// @return 割合
		float ConvertFrameToRatio(u_int _frame);

		/// @brief フレーム数で調整するか取得
		/// @return フレーム数で調整するか？
		bool UseFrame();


		void ImGuiDebug() override;
	};

	// コンセプトで取得できる
	template<typename T>
	concept AnimNotifyConcept = std::is_base_of_v<AnimationNotify_Base, T>;

}