#include "pch.h"
#include "PlayerMove.h"

void PlayerMove::Input()
{
	GamePad* pad = MainApplication::GetInstance()->GamePad();
	if (pad->ButtonPress(GamePad::Button::Arrow_Up))
	{
		moveVec.y = 1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Down))
	{
		moveVec.y = -1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Right))
	{
		moveVec.x = 1.0f;
	}
	if (pad->ButtonPress(GamePad::Button::Arrow_Left))
	{
		moveVec.x = -1.0f;
	}
}

void PlayerMove::Init()
{
}

void PlayerMove::Update()
{
	Input();	// “ü—Í
}

void PlayerMove::LateUpdate()
{
}

void PlayerMove::Draw()
{
}
