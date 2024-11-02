#pragma once

/// @brief ヒットストップで止まる側の基底クラス
class HitStopper_Base
{

public:
	HitStopper_Base() {}
	virtual ~HitStopper_Base();

	/// @brief ヒットストップマネージャーに自身を追加する
	void AddToHitStopManager();

	/// @brief ヒットストップ開始した時の処理
	virtual void OnHitStopBegin() = 0;

	/// @brief ヒットストップ終了した時の処理
	virtual void OnHitStopEnd() = 0;

private:
	/// @brief ヒットストップマネージャーから自身を削除する
	void RemoveFromHitStopManager();
};

