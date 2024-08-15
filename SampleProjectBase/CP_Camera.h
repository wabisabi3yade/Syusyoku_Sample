#pragma once
#include "Component.h"

// ゲーム内カメラクラス
class CP_Camera : public Component
{
	/// @brief 注視点
	DirectX::SimpleMath::Vector3 focusPos{ 0.0f, 0.0f, 0.0f };

	/// @brief カメラの上ベクトル
	DirectX::SimpleMath::Vector3 camUp{ Vec3::Up };

	/// @brief 視野角
	float fov;

	/// @brief 描画最短
	float nearZ;

	/// @brief 描画最長
	float farZ;

	/// @brief 平行投影にするか？
	bool isOrthographic{ false };

	// ビューポートの番号
	u_int viewPortSlot;
public:
	CP_Camera();
	~CP_Camera() {}

	void Init() override;

	void LateUpdate() override;

	void ImGuiSetting() override;

	/// @brief ビュー変換行列
	void UpdateViewMatrix();	

	/// @brief 透視投影に変更
	void SetPerspective();

	/// @brief 平行投影に変更
	void SetOrthographic();

	/// @brief 注視点セット
	/// @param _focusPos ワールド座標
	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);

	// ビューポート番号をセット
	void SetViewportSlot(u_int _slot);

private:
	/// @brief 透視投影行列を更新
	void UpdatePerspective();

	/// @brief 平行投影をセットする
	void UpdateOrthographic();
};

