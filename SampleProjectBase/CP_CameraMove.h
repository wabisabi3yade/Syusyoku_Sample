#pragma once
#include "CloneComponent.h"

class CP_Camera;

class CP_CameraMove : public Component,  public CloneComponent<CP_CameraMove>
{
	/// @brief 追従対象
	const GameObject* pTargetObj;

	/// @brief カメラ
	CP_Camera* pCamera;

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

	virtual void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void UpdateVector();
	void Move();
	void LookUpdate();
};

