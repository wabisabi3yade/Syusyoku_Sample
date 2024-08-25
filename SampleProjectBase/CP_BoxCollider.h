#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider, public CloneComponent<CP_BoxCollider>
{
	/// @brief 中心座標
	DirectX::SimpleMath::Vector3 centerPos;

	/// @brief 長さ
	DirectX::SimpleMath::Vector3 length;

	/// @brief 座標オフセット
	DirectX::SimpleMath::Vector3 posOffset;

	// AABBフラグ
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void LateUpdate() override;

	void ImGuiSetting() override;

	void SetOffset(const DirectX::SimpleMath::Vector3& _offset) { posOffset = _offset; } 

	// ワールド座標系へ変換
	DirectX::SimpleMath::Vector3 GetCenterPos()const;
	DirectX::SimpleMath::Vector3 GetLength()const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Start();
	/// @brief ボックスの形状を作成
	void CreateBoxShape();

	/// @brief モデルのサイズから当たり判定の大きさを決める
	void SizeFromModelSize();

	/// @brief 座標更新
	void PosUpdate();

	/// @brief 長さ更新
	void LengthUpdate();
};

