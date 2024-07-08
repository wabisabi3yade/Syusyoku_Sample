#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// コンポーネント	
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

	// 移動に左入力
	move->DecideMoveVector(input->GetValue("Left"));

	// 弾発射にR1,R2、Space入力
	using enum GamePad::Button;
	if (pad.ButtonPress(R1) || pad.ButtonPress(R2) || keyboard.GetKeyDown(DIK_SPACE))
		shot->Shot();
}
