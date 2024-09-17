#pragma once

class Sprite;
class Material;

/// @brief スプライトを描画する機能
class SpriteDrawer
{

public:
	SpriteDrawer() {}
	~SpriteDrawer() {}

	/// @brief スプライト描画
	/// @param _sprite スプライト
	/// @param _material マテリアル
	/// @param _pos ワールド座標
	/// @param _scale ワールドスケール
	/// @param _rotation ワールド回転量
	void DrawSprite(
		const Sprite& _sprite, 
		Material& _material, 
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale, 
		const DirectX::SimpleMath::Quaternion& _rotation
	);
};

