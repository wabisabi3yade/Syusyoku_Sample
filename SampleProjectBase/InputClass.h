#pragma once
#include "GamePad.h"
#include "GameKey.h"

namespace HashiTaku
{
	// �Q�[���̓��͑S�Ă����b�v����N���X
	class InputClass
	{
	public:
		/// @brief ���͕���
		enum class InputDirection
		{
			Up,
			Down,
			Right,
			Left,
			MaxNum
		};

	private:
		std::unique_ptr<GamePad> gamePad;	// �Q�[���p�b�h
		std::unique_ptr<GameKey> keyboard;	// �L�[�{�[�h

	public:
		InputClass();
		~InputClass();

		void Init(HWND _hwnd);

		void Update();	// �X�V

		GamePad& GetGamePad()const { return *gamePad.get(); }	// �R���g���[������

		const GameKey& GetKeyboard()const { return *keyboard.get(); }	// �L�[�{�[�h����

		/// @brief ���͂ŕ����̃{�^���������ꂽ���擾����
		/// @param _getDir �擾����������
		/// @return �������u�Ԃ��H
		bool GetInputTriggerDir(InputDirection _getDir) const;
	};
}
