#pragma once
#include "CloneComponent.h"

// ゲーム内カメラクラス
class CP_Camera : public Component, public CloneComponent<CP_Camera>
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
	~CP_Camera() {}

	void Init() override;

	void LateUpdate() override;

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief ビュー変換行列
	void UpdateViewMatrix();	

	/// @brief 透視投影に変更
	void SetPerspective();

	/// @brief 平行投影に変更
	void SetOrthographic();

	// ビューポート番号をセット
	void SetViewportSlot(u_int _slot);

	/// @brief セーブする
	/// @param _sceneData セーブデータ
	nlohmann::json Save();

	/// @brief ロードする
	/// @param _sceneData ロードするデータ 
	void Load(const nlohmann::json& _data);

private:
	/// @brief 透視投影行列を更新
	void UpdatePerspective();

	/// @brief 平行投影をセットする
	void UpdateOrthographic();
};

