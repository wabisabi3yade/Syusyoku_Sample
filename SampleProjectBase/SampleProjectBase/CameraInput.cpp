#include "pch.h"
#include "CameraInput.h"
#include "GameObject.h"
#include "CameraMove.h"
#include "GamePad.h"
using namespace DirectX::SimpleMath;

void CameraInput::Init()
{
	camMove = gameObject->GetComponent<CameraMove>();
}

void CameraInput::Update()
{
	// “ü—Í
	GamePad* pad = MainApplication::GamePad();
	Vector2 input = Vector2(pad->GetValue(GamePad::Value::StickR_X),
		pad->GetValue(GamePad::Value::StickR_Y));

	if (input.x != 0.0f || input.y != 0.0f)
		camMove->Roll(input);	// ƒJƒƒ‰‚Ì‰ñ“]
}
