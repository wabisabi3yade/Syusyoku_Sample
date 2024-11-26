#pragma once
#include "AnimationParameterType.h"

class AnimTransitionArrow;
class AnimationParameters;

/// @brief �J�ږ�󂪒B�����Ă��邩�m�F����
class AnimTransitionChecker
{
	/// @brief �A�j���[�V�����J�ڏ�����
	struct TransitionInfo
	{
		const AnimTransitionArrow* pArrow{ nullptr };	// �J�ږ��
		std::vector<const HashiTaku::AnimParam::conditionValType*> values; // ���̏����p�����[�^
	};

	/// @brief �J�ږ��ƃp�����[�^�̃A�h���X
	std::list<TransitionInfo> transitionInfos;

public:
	/// @brief �R���X�g���N�^
	/// @param _animParams �A�j���[�V�����p�����[�^
	/// @param _transArrows �J�ږ�󃊃X�g
	/// @param _groupArrows �O���[�v�ŊǗ����Ă���J�ږ��(nullptr�Ȃ�Ȃ�)
	AnimTransitionChecker(const AnimationParameters& _animParams,
		const std::list<std::unique_ptr<AnimTransitionArrow>>& _transArrows,
		const std::list<std::unique_ptr<AnimTransitionArrow>>* _pGroupTransArrows
	);
	~AnimTransitionChecker() {}

	/// @brief ��󂩂�p�����[�^�̃A�h���X���擾����
	/// @param animParams �A�j���[�V�����p�����[�^
	/// @param _pArrow ���
	void CreateInfo(const AnimationParameters& animParams, const AnimTransitionArrow& _pArrow);

	/// @brief �J�ڂł��邩�m�F
	/// @param  _curRatio ���݊���
	/// @param  _lastRatio 1�t���[���O����
	/// @return �J�ڏ�����B�������J�ږ��(�Ȃ��Ȃ�nullptr)
	const AnimTransitionArrow* TransitonCheck(float _curRatio, float _lastRatio);

private:
	/// @brief �J�ږ��̗D��x�Ń\�[�g����
	/// @param _a1 ���1
	/// @param _a2 ���2
	/// @return ����ւ�
	static bool SortArrowPriority(const TransitionInfo& _a1, const TransitionInfo& _a2);
};

