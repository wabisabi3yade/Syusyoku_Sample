#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// �R���|�[�l���g	
#include "PlayerMove.h"

using namespace DirectX::SimpleMath;

void PlayerInput::Init()
{
	input = &MainApplication::GetInput();
	move = gameObject->GetComponent<PlayerMove>();
}

void PlayerInput::Update()
{	
	const GamePad& pad = input->GetGamePad();
	const GameKey& keyboard = input->GetKeyboard();

	// �ړ��ɍ�����
	move->DecideMoveVector(input->GetValue("Left"));
}
