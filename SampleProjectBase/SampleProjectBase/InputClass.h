#pragma once
#include "GamePad.h"

// �Q�[���̓��͑S�Ă����b�v����N���X
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// �Q�[���p�b�h

public:
	InputClass();	
	~InputClass();

	void Update();	// �X�V

	GamePad* GetGamePad()const { return gamePad.get(); }
};

