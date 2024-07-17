#pragma once
#include "Component.h"

// ゲーム内カメラクラス
class Camera : public Component
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
public:
	using Component::Component;

	void Init() override;

	/// @brief ビュー変換行列
	void UpdateViewMatrix();	

	/// @brief 透視投影に変更
	/// @param _viewPortSlot 
	void SetPerspective(u_int _viewPortSlot = 0);

	/// @brief 平行投影に変更
	/// @param _viewPortSlot ビューポートインデックス
	void SetOrthographic(u_int _viewPortSlot = 0);

	/// @brief 注視点セット
	/// @param _focusPos ワールド座標
	void SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos);

private:
	/// @brief 透視投影行列を更新
	/// @param _viewPortSlot ビューポートインデックス
	void UpdatePerspective(u_int _viewPortSlot);

	/// @brief 平行投影をセットする
	/// @param _viewPortSlot ビューポートインデックス
	void UpdateOrthographic(u_int _viewPortSlot);
};

