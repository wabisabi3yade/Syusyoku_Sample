#pragma once
#include "AnimationParameterType.h"

class AnimTransitionArrow;
class AnimationParameters;

/// @brief 遷移矢印が達成しているか確認する
class AnimTransitionChecker
{
	/// @brief アニメーション遷移条件で
	struct TransitionInfo
	{
		const AnimTransitionArrow* pArrow{ nullptr };	// 遷移矢印
		std::vector<const HashiTaku::AnimParam::conditionValType*> values; // 矢印の条件パラメータ
	};

	/// @brief 遷移矢印とパラメータのアドレス
	std::list<TransitionInfo> transitionInfos;

public:
	/// @brief コンストラクタ
	/// @param _animParams アニメーションパラメータ
	/// @param _transArrows 遷移矢印リスト
	/// @param _groupArrows グループで管理している遷移矢印(nullptrならなし)
	AnimTransitionChecker(const AnimationParameters& _animParams,
		const std::list<std::unique_ptr<AnimTransitionArrow>>& _transArrows,
		const std::list<std::unique_ptr<AnimTransitionArrow>>* _pGroupTransArrows
	);
	~AnimTransitionChecker() {}

	/// @brief 矢印からパラメータのアドレスを取得する
	/// @param animParams アニメーションパラメータ
	/// @param _pArrow 矢印
	void CreateInfo(const AnimationParameters& animParams, const AnimTransitionArrow& _pArrow);

	/// @brief 遷移できるか確認
	/// @param  _curRatio 現在割合
	/// @param  _lastRatio 1フレーム前割合
	/// @return 遷移条件を達成した遷移矢印(ないならnullptr)
	const AnimTransitionArrow* TransitonCheck(float _curRatio, float _lastRatio);

private:
	/// @brief 遷移矢印の優先度でソートする
	/// @param _a1 矢印1
	/// @param _a2 矢印2
	/// @return 入れ替え
	static bool SortArrowPriority(const TransitionInfo& _a1, const TransitionInfo& _a2);
};

