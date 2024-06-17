#include "pch.h"
#include "InputClass.h"

InputClass::InputClass()
{
	gamePad = std::make_unique<GamePad>();
}

InputClass::~InputClass()
{
}

void InputClass::Update()
{
	gamePad->InputUpdate();
}
