#pragma once
#include "CP_Collider.h"

class CP_SphereCollider : public CP_Collider
{
	// 半径
	float radius;	
public:
	CP_SphereCollider() : CP_Collider(CP_Collider::ShapeType::Sphere), radius(0.5f) {}
	~CP_SphereCollider(){};
	
	// コンポーネント共通関数
	void Draw() override;

	/// @brief 半径を取得する
	/// @return 半径
	float GetRadius() const;

	void ImGuiSetting() override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief 形状作成
	void CreateShape() override;

	/// @brief 半径をセット
	void SetRadius(float _radius);

	/// @brief オブジェクトのスケール値を適用する
	/// @param _outRadius 計算した半径
	void ApplyObjectScale(float& _outRadius);
};

