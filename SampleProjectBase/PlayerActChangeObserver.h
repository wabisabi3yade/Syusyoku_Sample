#pragma once
#include "IObserever.h"

class PlayerGroundActionController;

/// @brief �v���C���[�̃A�N�V������Ԃ�J�ڂ���Ƃ��ɒʒm�����N���X
class PlayerActChangeObserver : public HashiTaku::IObserver<int>
{
	PlayerGroundActionController* pActionController;
public:
	/// @brief �R���X�g���N�^
	/// @param _observerName �I�u�U�[�o�[��
	/// @param _actionController �A�N�V�����R���g���[���[
	PlayerActChangeObserver(const std::string& _observerName, PlayerGroundActionController& _actionController);
	~PlayerActChangeObserver() {}

	void ObserverUpdate(const int& _changeType);
};

