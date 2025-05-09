#include "pch.h"
#include "AnimTransitionChecker.h"
#include "AnimTransitionArrow.h"

namespace HashiTaku
{
	AnimTransitionChecker::AnimTransitionChecker(const AnimationParameters& _animParams,
		const std::list<std::unique_ptr<AnimTransitionArrow>>& _transArrows,
		const std::list<std::unique_ptr<AnimTransitionArrow>>* _pGroupTransArrows)
	{
		// 遷移条件を作成
		for (auto& arrow : _transArrows)
			CreateInfo(_animParams, *arrow);

		// グループ側の条件があるなら
		if (_pGroupTransArrows)
		{
			// 遷移条件を作成
			for (auto& groupArrow : *_pGroupTransArrows)
				CreateInfo(_animParams, *groupArrow);
		}

		// 優先度でソートする
		transitionInfos.sort(SortArrowPriority);
	}

	void AnimTransitionChecker::CreateInfo(const AnimationParameters& _animParams, const AnimTransitionArrow& _arrow)
	{
		using namespace AnimParam;

		TransitionInfo transInfo;
		transInfo.pArrow = &_arrow;

		// コントローラー側から名前を取得し、パラメータのアドレスを取得する
		auto& conditionList = _arrow.GetConditionList();
		for (auto& cond : conditionList)
		{
			const conditionValType* pVal = _animParams.GetValueAddress(cond->GetReferenceParamName());
			assert(pVal && "正常にパラメータのアドレスが取得できませんでした");

			transInfo.values.push_back(pVal);
		}

		// 追加
		transitionInfos.push_back(std::move(transInfo));
	}

	const AnimTransitionArrow* AnimTransitionChecker::TransitonCheck(float _curRatio,
		float _lastRatio)
	{
		for (auto& transInfo : transitionInfos)	// 矢印の数
		{
			// 活動していないなら次へ
			if (!transInfo.pArrow->GetIsActive()) continue;

			auto& conditionList = transInfo.pArrow->GetConditionList();
			auto condItr = conditionList.begin();

			bool isNotArchive = false;	// 達成していない条件があるか？

			u_int valCnt = static_cast<u_int>(transInfo.values.size());
			for (u_int t_i = 0; t_i < valCnt; t_i++)	// 遷移条件の数
			{
				// 変数を代入して条件を達成しているか確認
				auto pValue = transInfo.values[t_i];

				if (!(*condItr)->IsCondition(*pValue))	// 達成していないなら
				{
					isNotArchive = true;
					break;
				}

				++condItr;	// 次の条件に進
			}

			if (!isNotArchive)	// 全て達成しているなら
			{

				if (transInfo.pArrow->GetIsHasExit())	// 遷移終了時間を持っているなら
				{
					float exitRatio = transInfo.pArrow->GetExitRatio();
					if (_curRatio < exitRatio || _lastRatio >= exitRatio)	// 遷移終了時間をまたいでいないなら
						continue;
				}
				return transInfo.pArrow;
			}
		}

		return nullptr;	// 達成していないなら
	}

	bool AnimTransitionChecker::SortArrowPriority(const TransitionInfo& _a1,
		const TransitionInfo& _a2)
	{
		return _a1.pArrow->GetPriority() > _a2.pArrow->GetPriority();
	}
}