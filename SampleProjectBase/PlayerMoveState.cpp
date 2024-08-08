#include "pch.h"
#include "PlayerMoveState.h"

#include "Camera.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;

PlayerMoveState::PlayerMoveState(PlayerActionController& _controller, GameObject& _gameObject) : PlayerState_Base(_controller, _gameObject), moveSpeed(10.0f)
{
}

void PlayerMoveState::Init()
{

}

void PlayerMoveState::Update()
{
	Move();
}

void PlayerMoveState::Terminal()
{
}

void PlayerMoveState::Move()
{
	Vector2 inputValue = MainApplication::GetInput().GetValue("Left");

	CP_Camera& camera = InSceneSystemManager::GetInstance()->GetMainCamera();
	Vector3 camForwardVec = camera.GetTransform().Forward();
	Vector3 camRightVec = camera.GetTransform().Right();

	moveVector.x = camRightVec.x * inputValue.x;
	moveVector.z = camForwardVec.z * inputValue.y;
	moveVector.y = 0.0f;
	moveVector.Normalize();

	HASHI_DEBUG_LOG("x:" + std::to_string(moveVector.x));
	HASHI_DEBUG_LOG("z:" + std::to_string(moveVector.z));

	Transform& transform = pGameObject->transform;

	transform.position += moveVector * moveSpeed * MainApplication::DeltaTime();
}
