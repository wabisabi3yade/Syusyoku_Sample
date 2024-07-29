#pragma once
#include "Component.h"

class Camera;
class CameraMove : public Component
{
	const GameObject* pPlayer{ nullptr };	// プレイヤーのオブジェクト

	// カメラクラス
	Camera* camera{ nullptr };

	// カメラの移動速度
	float moveSpeed{ 0.0f };

	/// @brief 速いときの速度
	float dashSpeed{ 0.0f };

	// 視点移動速度
	float lookSpeed{ 0.0f };

	// 移動方向ベクトル
	DirectX::SimpleMath::Vector3 moveVec;

	// 視点移動ベクトル
	DirectX::SimpleMath::Vector3 rotateVec;

public:
	using Component::Component;

	virtual void Init();
	virtual void LateUpdate();

	virtual void ImGuiSetting() override;

private:
	void UpdateVector();
	void Move();
};

