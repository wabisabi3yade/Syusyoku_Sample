#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// コンポーネント	
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

	// 移動に左入力
	move->DecideMoveVector(input->GetValue("Left"));
}
