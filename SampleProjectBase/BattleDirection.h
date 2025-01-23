#pragma once
#include "Component.h"
#include "AnimationCurve.h"
#include "IObserever.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	class CP_Fade;
	class CP_CameraMove;

	/// @brief 戦闘の演出を行う
	class BattleDirection : public ISaveLoad, public IImGuiUser, public Subject<int>
	{
	public:
		/// @brief バトルの状態
		enum class DirectionState
		{
			Wait,
			Start,	// 最初の演出
			Win,	// 勝利
			Lose	// 負け
		};

	private:
		/// @brief ゲームスタートの速度カーブ
		AnimationCurve gameStartCurve;

		/// @brief 勝利時に揺らすパラメータ
		PerlinShakeParameter winShakeParam;

		/// @brief フェードオブジェクト名
		std::string fadeObjName;

		/// @brief バトルスタートオブジェクト名
		std::string startObjName;

		/// @brief Winオブジェクト名
		std::string winObjName;

		/// @brief ゲームオーバー名
		std::string gameOverObjName;

		/// @brief フェード
		CP_Fade* pFade;

		/// @brief バトルスタートオブジェクト
		GameObject* pGameStartObj;

		/// @brief 勝利文字
		CP_Fade* pWinObj;

		/// @brief 敗北文字
		CP_Fade* pGameOverObj;

		/// @brief カメラ移動クラス
		CP_CameraMove* pCamMove;

		/// @brief 現在の状態
		DirectionState curState;

		/// @brief 演出の経過時間
		float animationElapsedTime;

		/// @brief 勝利時のタイムスケール
		float winTimeScale;

		/// @brief 演出の段階
		int directionStep;

		/// @brief UIのアニメーションを行うか？
		bool isUIAnimation;

		/// @brief 開始時のフェード中か？
		bool isStartFading;
	public:
		BattleDirection();
		~BattleDirection() {}

		/// @brief 初期処理
		void Init(CP_CameraMove* _pCamMove);

		/// @brief 更新処理
		/// @param _deltaTime 経過時間
		void Update(float _deltaTime);

		/// @brief 開始の演出を始める
		void OnBeginStartDirection();

		/// @brief 勝利時の演出を始める
		/// @param _targetTransform カメラが映すターゲットのトランスフォーム
		void OnBeginWinDirection(const Transform* _targetTransform);

		/// @brief 敗北時の演出を始める
		/// @param _targetTransform カメラが映すターゲットのトランスフォーム
		void OnBeginLoseDirection(const Transform* _targetTransform);

		/// @brief 現在の演出の状態を取得する
		/// @return 現在の演出の状態
		DirectionState GetDirectionState() const;

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	private:
		/// @brief オブジェクトを探す
		void FindObject();

		/// @brief スタート状態開始時の処理
		void OnBeginStart();
		
		/// @brief 開始時の更新処理
		/// @param _deltaTime 経過時間
		void StartUpdate(float _deltaTime);

		/// @brief 勝利時の更新処理
		/// @param _deltaTime 経過時間
		void WinUpdate(float _deltaTime);

		/// @brief 敗北時の更新処理
		/// @param _deltaTime 経過時間
		void LoseUpdate(float _deltaTime);

		/// @brief 演出終了の処理
		void OnEndDirection();

		void ImGuiDebug() override;
		// 勝利時の演出
		void ImGuiWinDirection();
		// 必要なオブジェクトの名前を編集
		void ImGuiSetObject();
	};
}


