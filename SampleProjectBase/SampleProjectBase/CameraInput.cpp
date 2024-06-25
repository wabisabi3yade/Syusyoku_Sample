#include "pch.h"
#include "CameraInput.h"
#include "GameObject.h"
#include "CameraMove.h"
#include "GamePad.h"
using namespace DirectX::SimpleMath;

void CameraInput::Init()
{
	name = "CameraInput";
	camMove = gameObject->GetComponent<CameraMove>();
}

void CameraInput::Update()
{
	// “ü—Í
	InputClass& input = MainApplication::GetInput();
	Vector2 inputVal = input.GetValue("Right");

	if(inputVal != Vector2::Zero)
	camMove->Roll(inputVal);	// ƒJƒƒ‰‚Ì‰ñ“]
}
