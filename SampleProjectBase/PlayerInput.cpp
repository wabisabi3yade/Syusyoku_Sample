#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// �R���|�[�l���g	
#include "CP_PlayerMove.h"

using namespace DirectX::SimpleMath;

void PlayerInput::Init()
{
	input = &MainApplication::GetInput();
	move = gameObject->GetComponent<CP_PlayerMove>();
}

void PlayerInput::Update()
{	

}
