#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief カメラゲームオーバー処理
	class CameraGameOverState : public CameraMoveState_Base
	{
		/// @brief カメラを移すトランスフォーム
		const Transform* pTargetTransform;

		/// @brief オフセット
		DXSimp::Vector2 lookOffset;

		/// @brief 横の距離
		float horiDistance;

		/// @brief 高さ
		float heightDistance;
	public:
		CameraGameOverState();
		~CameraGameOverState() {}

		void OnStartBehavior() override;

		void UpdateBehavior() override;

		/// @brief ターゲットのトランスフォームをセット
		/// @param _targetPos ターゲットのトランスフォームの参照
		void SetTargetTransform(const Transform& _targetTransform);

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}