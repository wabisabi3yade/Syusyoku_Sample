#pragma once

class AnimationParameters;

/// @brief アニメーションパラメータセットインターフェース
class IAnimParametersSetter
{
public:
	IAnimParametersSetter() {}
	virtual ~IAnimParametersSetter() {}

	/// @brief アニメーションパラメータセット
	/// @param _animatiionParameters アニメーションパラメータ
	virtual void SetAnimationParameters(AnimationParameters& _animatiionParameters) = 0;
};

