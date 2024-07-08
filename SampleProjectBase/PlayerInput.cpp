#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// �R���|�[�l���g	
#include "PlayerMove.h"
#include "PlayerShot.h"

using namespace DirectX::SimpleMath;

void PlayerInput::Init()
{
	input = &MainApplication::GetInput();
	move = gameObject->GetComponent<PlayerMove>();
	shot = gameObject->GetComponent<PlayerShot>();
}

void PlayerInput::Update()
{	
	const GamePad& pad = input->GetGamePad();
	const GameKey& keyboard = input->GetKeyboard();

	// �ړ��ɍ�����
	move->DecideMoveVector(input->GetValue("Left"));

	// �e���˂�R1,R2�ASpace����
	using enum GamePad::Button;
	if (pad.ButtonPress(R1) || pad.ButtonPress(R2) || keyboard.GetKeyDown(DIK_SPACE))
		shot->Shot();
}
