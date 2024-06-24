#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{nullptr};	// プレイヤーのオブジェクト
	Camera* camera{ nullptr };
	//Roll
	float rotateSpeed{ 0.0f };	// 回転速度
	float centerAngle{ 0.0f };	// 中心とのワールド座標系角度
	float heightSpeed{ 0.0f };	// 高さ速度
	float maxHeight{ 0.0f };	// 高さの最大値（プレイヤーから）
	float minHeight{ 0.0f };	// 高さの最小値
	float nowHeight{ 0.0f };	//	現在の高さ

	// UpdatePosition
	float moveSpeed{ 0.0f };	// カメラの移動速度
	float playDistance{ 0.0f };	// プレイヤーとの遊びの距離
	float distance{ 0.0f };	// プレイヤーとの距離(XZ平面)

	// Updatelook
	float lookSpeed{ 0.0f };	// 視点移動速度
	float lookOffsetY{0.0f};	// Y軸オフセット
	DirectX::SimpleMath::Vector3 lookPos;	// 現在の注視点
	
	void UpdatePosition();	// カメラの座標を更新する
	void UpdateLook();	// カメラの注視点を更新する
	
public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	void Roll(DirectX::SimpleMath::Vector2 _input);	// コントローラー入力からカメラを回転する

	virtual void SetParameter();
	void SetPlayer(GameObject& _player) { pPlayer = &_player; }
};

