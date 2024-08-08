#include "pch.h"
#include "PlayerInput.h"
#include "GameObject.h"

// コンポーネント	
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
