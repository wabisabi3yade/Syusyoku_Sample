#pragma once
#include "StateMachine.h"
#include "CP_Camera.h"
#include "GameInput.h"

namespace HashiTaku
{
	class CameraMoveController;

	/// @brief カメラの行動基底クラス
	class CameraMoveState_Base : public StateNode_Base,
		public ISaveLoad, public IImGuiUser
	{
	public:
		enum  class CameraState
		{
			Follow,	// 移動時
			Charge,	// チャージ時
			Win,	// プレイヤー勝利
			Lose,	// プレイヤー負け
			None
		};		
	protected:
		/// @brief カメラの行動コントローラ
		CameraMoveController* pCamController;

		/// @brief 入力
		GameInput* pInput;

	public:
		CameraMoveState_Base();
		virtual ~CameraMoveState_Base() {}

		/// @brief 初期化処理
		/// @param _cameraController カメラコントローラ 
		void Init(CameraMoveController& _cameraController);

		/// @brief 状態切り替え開始処理
		void OnStart() override;

		/// @brief 更新処理
		void Update() override;

		/// @brief 状態切り替え終了処理
		void OnEnd() override;

		json Save() override;
		void Load(const json& _data) override;
	protected:
		/// @brief 開始処理の振る舞い
		virtual void OnStartBehavior() {}

		/// @brief 更新処理の振る舞い
		virtual void UpdateBehavior() {}

		/// @brief 終了処理の振る舞い
		virtual void OnEndBehavior() {}

		/// @brief 他のステートに遷移するか確認
		virtual void CheckTransitionUpdate();

		/// @brief カメラのステートを遷移
		/// @param _camState 遷移先のカメラ
		void ChangeState(CameraState _camState, bool _isForce = false);

		/// @brief ベースとなるカメラの座標をセット
		/// @param _basePos ベースとなる座標
		void SetBasePosition(const DXSimp::Vector3& _basePos);

		/// @brief カメラコンポーネントを取得
		/// @return カメラ
		CP_Camera& GetCamera();

		/// @brief 追従先の座標を取得する
		/// @return  追従先の座標
		const DXSimp::Vector3& GetFollowPosition() const;

		/// @brief ベースとなる座標を取得する
		/// @return	カメラのベースとなるワールド座標
		const DXSimp::Vector3& GetBasePosition() const;

		/// @brief オブジェクトの経過時間を取得
		/// @return 経過時間
		float DeltaTime() const;

		void ImGuiDebug() override;
	};
}