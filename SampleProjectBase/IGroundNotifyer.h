#pragma once

/// @brief 地面判定通知してもらう側インターフェース
class IGroundNotifyer
{

public:
	IGroundNotifyer() = default;
	~IGroundNotifyer() = default;

	/// @brief 地面に付いた瞬間の処理
	virtual void OnGroundEnter() = 0;

	/// @brief 地面から離れた瞬間の処理
	virtual void OnGroundExit() = 0;
};