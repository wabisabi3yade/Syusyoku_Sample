#pragma once
#include "CP_Collider.h"

/// @brief カプセルコライダー
class CP_CapsuleCollider : public CP_Collider
{
	/// @brief 半径
	float radius;

	/// @brief 高さ
	float height;
public:
	CP_CapsuleCollider();
	~CP_CapsuleCollider() {}

	/// @brief 半径をセット
	/// @param _setRadius 半径
	void SetRadius(float _setRadius);

	/// @brief 高さをセット
	/// @param _setHeight 高さ
	void SetHeight(float _setHeight);

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief ボックスの形状を作成
	void CreateShape() override;

	/// @brief オブジェクトのスケール値を反映
	/// @param _outRadius 反映した半径
	/// @param _outHeight 反映した高さ
	void ApplyObjectScale(float& _outRadius, float& _outHeight);

	void ImGuiSetting() override;
};

