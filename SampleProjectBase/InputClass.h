#pragma once
#include "GamePad.h"
#include "GameKey.h"

// �Q�[���̓��͑S�Ă����b�v����N���X
class InputClass
{
	enum class Move_Value
	{
		Up,
		Down,
		Right,
		Left,
		MaxNum
	};

	std::unique_ptr<GamePad> gamePad;	// �Q�[���p�b�h
	std::unique_ptr<GameKey> keyboard;	// �L�[�{�[�h
	
	/// @brief �ړ��֘A�͂�
	std::array<bool, static_cast<u_int>(Move_Value::MaxNum)> moveFlags;
public:
	InputClass();	
	~InputClass();

	void Init(HWND _hwnd);

	void Update();	// �X�V

	const GamePad& GetGamePad()const { return *gamePad.get(); }	// �R���g���[������

	const GameKey& GetKeyboard()const { return *keyboard.get(); }	// �L�[�{�[�h����
};

