#pragma once

/// @brief アニメーション間の補間処理
class IAnimInterpolate
{
public:
	IAnimInterpolate() {}
	~IAnimInterpolate() {}

	// 経過時間を取得する
	virtual float GetElapsedTime() const = 0;

	// 遷移時間を取得する
	virtual float GetTransitionTime() const = 0;
};