#pragma once
#include "Component.h"

class CP_Camera;

class CP_CameraMove : public Component
{
	/// @brief 追従対象
	GameObject* pTargetObj;

	/// @brief カメラ
	CP_Camera* pCamera;

	/// @brief 注視点のオフセット
	DirectX::SimpleMath::Vector3 offsetTarget;

	/// @brief カメラの移動速度	
	float rotateSpeed;

	/// @brief ターゲットとの中心角度
	float centerAngle;

	/// @brief ターゲットとの横距離
	float distanceHori;

	/// @brief ターゲットとの縦距離
	float distanceVer;
	
	/// @brief 視点移動ベクトル
	float rotateVec;

public:
	CP_CameraMove();
	~CP_CameraMove() {}

	virtual void Start();
	virtual void LateUpdate();

	virtual void ImGuiDebug() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void UpdateVector();
	void Move();
	void LookUpdate();

	bool IsCanUpdate();
};

