#pragma once
#include "CameraMoveState_Base.h"

namespace HashiTaku
{
	/// @brief 勝利時のカメラ演出(Devil May Cry5風で)
	class CameraWinState : public CameraMoveState_Base
	{
		enum class DirectionStep
		{
			Move, // 移動
			Zoom, // ズーム
			Wait, // 待機
			End	// 終了
		};

		/// @brief カメラに映す対象のトランスフォーム
		const Transform* pTargetTransform;

		/// @brief 映す対象の座標から中心座標までののオフセット
		DXSimp::Vector3 centerPosOffset;

		/// @brief カメラを向ける対象の回転量
		DXSimp::Quaternion lookTargetRotation;

		/// @brief 演出開始時のカメラの回転量
		DXSimp::Quaternion beginCamRotation;

		/// @brief 移動先の座標
		DXSimp::Vector3 moveTargetPos;

		/// @brief 演出開始時のカメラの座標
		DXSimp::Vector3 beginCamPos;

		/// @brief ランダムで求めるターゲットまでのベクトルの範囲（最大）
		DXSimp::Vector3 randTargetVecMax;

		/// @brief ランダムで求めるターゲットまでのベクトルの範囲（最小）
		DXSimp::Vector3 randTargetVecMin;

		/// @brief 現在のステップ
		DirectionStep curStep;

		/// @brief 次に行うステップ
		DirectionStep nextStep;

		/// @brief 移動時のイージング
		EaseKind moveEaseKind;

		/// @brief 視点移動時のイージング
		EaseKind lookEaseKind;

		/// @brief 視点移動時の視野角イージング
		EaseKind fovEaseKindOnMove;

		/// @brief ズーム時の視野角イージング
		EaseKind fovEaseKindOnZoom;

		/// @brief ターゲットまでの距離
		float targetToDistance;

		/// @brief 移動演出時間
		float moveTime;

		/// @brief 待機演出時間(1回目)
		float firstWaitTime;

		/// @brief ズーム時の演出時間
		float zoomTime;

		/// @brief 待機演出時間(2回目)
		float secondWaitTime;

		/// @brief 現在設定されている待機時間
		float curTargetWaitTime;

		/// @brief 現在の経過時間
		float elapsedTime;

		/// @brief 移動時のターゲット視野角
		float targetFovOnMove;

		/// @brief ズーム時のターゲット視野角
		float targetFovOnZoom;

		/// @brief 開始時の視野角
		float beginFov;
	public:
		CameraWinState();
		~CameraWinState() {}

		/// @brief ターゲットのトランスフォームをセット
		/// @param _targetTransform ターゲットのトランスフォーム
		void SetTargetTransform(const Transform& _targetTransform);

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief 開始処理の振る舞い
		void OnStartBehavior() override;

		/// @brief 更新処理の振る舞い
		void UpdateBehavior() override;

		/// @brief 各ステップの開始処理を行う
		/// @param _nextStep 次のステップ
		void BeginDirection(DirectionStep _nextStep);

		/// @brief ターゲットまでのベクトルを乱数で決める
		/// @return 乱数で求めたベクトル
		DXSimp::Vector3 GetRandomTargetVec();

		/// @brief 移動時の開始処理
		void BeginMove();

		/// @brief 近づくときの処理
		void MoveUpdate();

		/// @brief 待機の開始処理
		/// @param _waitTime 待機時間
		/// @param _nextStep 待機が終わった後の次のステップ
		void BeginWait(float _waitTime, DirectionStep _nextStep);

		/// @brief 待機更新処理
		void WaitUpdate();

		/// @brief ズーム開始処理
		void BeginZoom();

		/// @brief ズーム処理
		void ZoomUpdate();

		/// @brief 終了時の処理
		void EndProcess();

		// パラメータ編集
		void ImGuiDebug() override;
		void ImGuiMove();
		void ImGuiZoom();
		void ImGuiWait();
	};
}

