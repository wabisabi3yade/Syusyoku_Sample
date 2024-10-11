#pragma once
#include "CP_Collider.h"

class CP_BoxCollider : public CP_Collider
{
	/// @brief 長さ
	DirectX::SimpleMath::Vector3 length;

	// AABBフラグ
	bool isAABB = false;	

public:
	CP_BoxCollider();
	~CP_BoxCollider() {}

	void ImGuiSetting() override;

	void SetLength(const DirectX::SimpleMath::Vector3& _length);

	DirectX::SimpleMath::Vector3 GetLength()const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Init();
	void LateUpdate() override;

	/// @brief ボックスの形状を作成
	void CreateShape() override;

	/// @brief モデルのサイズから当たり判定の大きさを決める
	void SizeFromModelSize();

	/// @brief 長さ更新
	void LengthUpdate();

	/// @brief ワールド座標に計算
	/// @param ワールド座標での長さ
	void CalcWorldLength(DirectX::SimpleMath::Vector3& _out);
};

