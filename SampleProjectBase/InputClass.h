#pragma once
#include "GamePad.h"
#include "GameKey.h"

// �Q�[���̓��͑S�Ă����b�v����N���X
class InputClass
{
	std::unique_ptr<GamePad> gamePad;	// �Q�[���p�b�h
	std::unique_ptr<GameKey> keyboard;	// �L�[�{�[�h

	// �p�b�h�ƃL�[�{�[�h���̗͂��Ή����������͒l���i�[����
	std::unordered_map<std::string, DirectX::SimpleMath::Vector2> inputValue;
public:
	InputClass();	
	~InputClass();

	void Init(HWND _hwnd);

	void Update();	// �X�V

	const GamePad& GetGamePad()const { return *gamePad.get(); }	// �R���g���[������

	const GameKey& GetKeyboard()const { return *keyboard.get(); }	// �L�[�{�[�h����
	
	DirectX::SimpleMath::Vector2 GetValue(const std::string& _getType)const;
};

