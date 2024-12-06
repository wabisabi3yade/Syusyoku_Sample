//#pragma once
//#include "AnimationNotifyState.h"
//#include "AnimationCurve.h"
//
///// @brief ルートモーションをターゲットの座標に移動させる通知イベント
//class ANS_WarpMotion : public AnimationNotifyState
//{
//	/// @brief ターゲットの座標へ移動する割合のカーブ
//	AnimationCurve moveRatioCurve;
//
//	/// @brief ターゲットの座標
//	DirectX::SimpleMath::Vector3 targetPos;
//	
//	/// @brief 1フレーム前の座標
//	DirectX::SimpleMath::Vector3 lastPos;
//
//	/// @brief 1フレーム前の割合値
//	float lastCurveValue;	
//public:
//	ANS_WarpMotion();
//	~ANS_WarpMotion() {}
//
//	/// @brief ターゲットの座標をセット
//	/// @param _targetPos ターゲットの座標
//	void SetTargetPosition(const DirectX::SimpleMath::Vector3& _targetPos);
//private:
//	void Begin() override;
//
//	/// @brief イベント間の更新処理
//	void Tick() override;
//};
//
