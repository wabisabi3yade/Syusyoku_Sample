#pragma once
#include "Component.h"

// ゲーム内カメラクラス
class CP_Camera : public Component
{
	/// @brief カメラの上ベクトル
	DirectX::SimpleMath::Vector3 camUp{ Vec3::Up };

	/// @brief 視野角
	float fov;
	
	/// @brief 描画距離
	float distance;

	/// @brief 平行投影にするか？
	bool isOrthographic{ false };

	// ビューポートの番号
	u_int viewPortSlot;
public:
	CP_Camera();
	~CP_Camera() {};

	void Init() override;
	void OnDestroy() override;

	/// @brief ビュー変換行列
	void UpdateViewMatrix();	

	/// @brief 透視投影に変更
	void SetPerspective();

	/// @brief 平行投影に変更
	void SetOrthographic();

	// fovセット
	void SetFov(float _setFov);

	// ビューポート番号をセット
	void SetViewportSlot(u_int _slot);

	void ImGuiDebug() override;

	/// @brief セーブする
	/// @param _sceneData セーブデータ
	nlohmann::json Save();

	/// @brief ロードする
	/// @param _sceneData ロードするデータ 
	void Load(const nlohmann::json& _data);

private:
	/// @brief 投影行列を更新する
	void UpdateProjection();

	/// @brief 透視投影行列を更新
	void UpdatePerspective();

	/// @brief 平行投影をセットする
	void UpdateOrthographic();
};

/// @brief CP_CameraのNullオブジェクト
class CP_CameraNull : public CP_Camera
{
public:
	CP_CameraNull() {}
	~CP_CameraNull() {}

	void Init() override {}

	void LateUpdate() override {}

	void Draw() override {}

	void ImGuiDebug() override {}

	/// @brief ビュー変換行列
	void UpdateViewMatrix() {}

	/// @brief 透視投影に変更
	void SetPerspective() {}

	/// @brief 平行投影に変更
	void SetOrthographic() {}

	// fovセット
	void SetFov(float _setFov) {}

	// ビューポート番号をセット
	void SetViewportSlot(u_int _slot) {}

	/// @brief セーブする
	/// @param _sceneData セーブデータ
	nlohmann::json Save() { return nlohmann::json(); }

	/// @brief ロードする
	/// @param _sceneData ロードするデータ 
	void Load(const nlohmann::json& _data) {}
};