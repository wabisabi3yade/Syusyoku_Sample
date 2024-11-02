#pragma once
#include "SingletonComponent.h"
#include "HitStopper_Base.h"

class CP_HitStopManager : public SingletonComponent<CP_HitStopManager>
{
private:
	friend class SingletonComponent<CP_HitStopManager>;

	/// @brief 登録されているヒットストップ
	std::list<HitStopper_Base*> hitStoppers;

	/// @brief 止めるフレーム数
	u_int stopFrame { 0 };

	/// @brief 現在の止めているフレーム数
	u_int curStopFrame { 0 };

	/// @brief ヒットストップ中か？
	bool isHitStopping { false };
public:
	/// @brief ヒットストッパーを追加する
	/// @param _addHitStopper 追加するヒットストッパー
	void AddHitStopper(HitStopper_Base& _addHitStopper);

	/// @brief ヒットストッパーを削除する
	/// @param _removeHitStopper 削除するヒットストッパー
	void RemoveHitStopper(HitStopper_Base& _removeHitStopper);

	/// @brief ヒットストップ開始
	/// @param _stopFrame 止めるフレーム数
	void HitStopBegin(u_int _stopFrame);
private:
	//CP_HitStopManager();
	/*~CP_HitStopManager() {}*/

	void Update() override;

	/// @brief ヒットストップ中の更新処理
	void HitStopUpdate();

	/// @brief ヒットストップ終了時の処理
	void OnHitStopEnd();
};

