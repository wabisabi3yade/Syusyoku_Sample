#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief  ターゲット時のカメラ
	class CameraTargetMove : public CameraMoveState_Base
	{
		/// @brief 注視するときの回転速度
		float targetLookRotSpeed;

		/// @brief プレイヤーとの距離
		float horiDisLength;

		/// @brief 追従カメラ速度
		float followMoveSpeed;

		/// @brief 追従先のオフセットY座標
		float followOffsetY;

		/// @brief 注視店をターゲットの座標からy座標分ずらす
		float targetLookOffsetY;

		/// @brief 追従先からの最大の高さ
		float maxHeight;

		/// @brief 追従先からの最低の高さ
		float minHeight;
	public:
		CameraTargetMove();
		~CameraTargetMove() {}

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理の振る舞い
		void OnStartBehavior() override;

		/// @brief 更新処理の振る舞い
		void UpdateBehavior()  override;

		/// @brief 終了処理の振る舞い
		void OnEndBehavior()  override;

		/// @brief 遷移移動
		void CheckTransitionUpdate() override;

		/// @brief 更新できるか取得
		/// @return 更新できるか
		bool CanUpdate();

		/// @brief プレイヤー追従する
		void FollowMove();

		/// @brief 視点の動き
		void LookMove();

		void ImGuiDebug() override;
	};
}