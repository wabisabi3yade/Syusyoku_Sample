#pragma once
#include "IObserever.h"

class PlayerActionController;

/// @brief �v���C���[�̃A�N�V������Ԃ�J�ڂ���Ƃ��ɒʒm�����N���X
class PlayerActChangeObserver : public HashiTaku::IObserver<int>
{
	PlayerActionController* pActionController;
public:
	/// @brief �R���X�g���N�^
	/// @param _observerName �I�u�U�[�o�[��
	/// @param _actionController �A�N�V�����R���g���[���[
	PlayerActChangeObserver(const std::string& _observerName, PlayerActionController& _actionController);
	~PlayerActChangeObserver() {}

	void Update(const int& _changeType);
};

