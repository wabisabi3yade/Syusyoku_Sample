#pragma once
#include "GamePad.h"
#include "GameKey.h"

// �Q�[���̓��͑S�Ă����b�v����N���X
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// �Q�[���p�b�h
	std::unique_ptr<GameKey> keyboard;	// �L�[�{�[�h
public:
	InputClass();	
	~InputClass();

	void Update();	// �X�V

	GamePad* GetGamePad()const { return gamePad.get(); }
};

