#include "pch.h"
#include "AnimTransitionChecker.h"
#include "AnimTransitionArrow.h"

namespace HashiTaku
{
	AnimTransitionChecker::AnimTransitionChecker(const AnimationParameters& _animParams,
		const std::list<std::unique_ptr<AnimTransitionArrow>>& _transArrows,
		const std::list<std::unique_ptr<AnimTransitionArrow>>* _pGroupTransArrows)
	{
		// �J�ڏ������쐬
		for (auto& arrow : _transArrows)
			CreateInfo(_animParams, *arrow);

		// �O���[�v���̏���������Ȃ�
		if (_pGroupTransArrows)
		{
			// �J�ڏ������쐬
			for (auto& groupArrow : *_pGroupTransArrows)
				CreateInfo(_animParams, *groupArrow);
		}

		// �D��x�Ń\�[�g����
		transitionInfos.sort(SortArrowPriority);
	}

	void AnimTransitionChecker::CreateInfo(const AnimationParameters& _animParams, const AnimTransitionArrow& _arrow)
	{
		using namespace AnimParam;

		TransitionInfo transInfo;
		transInfo.pArrow = &_arrow;

		// �R���g���[���[�����疼�O���擾���A�p�����[�^�̃A�h���X���擾����
		auto& conditionList = _arrow.GetConditionList();
		for (auto& cond : conditionList)
		{
			const conditionValType* pVal = _animParams.GetValueAddress(cond->GetReferenceParamName());
			assert(pVal && "����Ƀp�����[�^�̃A�h���X���擾�ł��܂���ł���");

			transInfo.values.push_back(pVal);
		}

		// �ǉ�
		transitionInfos.push_back(std::move(transInfo));
	}

	const AnimTransitionArrow* AnimTransitionChecker::TransitonCheck(float _curRatio,
		float _lastRatio)
	{
		for (auto& transInfo : transitionInfos)	// ���̐�
		{
			// �������Ă��Ȃ��Ȃ玟��
			if (!transInfo.pArrow->GetIsActive()) continue;

			auto& conditionList = transInfo.pArrow->GetConditionList();
			auto condItr = conditionList.begin();

			bool isNotArchive = false;	// �B�����Ă��Ȃ����������邩�H

			u_int valCnt = static_cast<u_int>(transInfo.values.size());
			for (u_int t_i = 0; t_i < valCnt; t_i++)	// �J�ڏ����̐�
			{
				// �ϐ��������ď�����B�����Ă��邩�m�F
				auto pValue = transInfo.values[t_i];

				if (!(*condItr)->IsCondition(*pValue))	// �B�����Ă��Ȃ��Ȃ�
				{
					isNotArchive = true;
					break;
				}

				++condItr;	// ���̏����ɐi
			}

			if (!isNotArchive)	// �S�ĒB�����Ă���Ȃ�
			{

				if (transInfo.pArrow->GetIsHasExit())	// �J�ڏI�����Ԃ������Ă���Ȃ�
				{
					float exitRatio = transInfo.pArrow->GetExitRatio();
					if (_curRatio < exitRatio || _lastRatio >= exitRatio)	// �J�ڏI�����Ԃ��܂����ł��Ȃ��Ȃ�
						continue;
				}
				return transInfo.pArrow;
			}
		}

		return nullptr;	// �B�����Ă��Ȃ��Ȃ�
	}

	bool AnimTransitionChecker::SortArrowPriority(const TransitionInfo& _a1,
		const TransitionInfo& _a2)
	{
		return _a1.pArrow->GetPriority() > _a2.pArrow->GetPriority();
	}
}