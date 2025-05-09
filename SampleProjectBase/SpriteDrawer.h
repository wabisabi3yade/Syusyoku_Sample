#pragma once

namespace HashiTaku
{
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
		/// @param _alpha α値
		void DrawSprite(
			const Sprite& _sprite,
			Material& _material,
			const DXSimp::Vector3& _pos,
			const DXSimp::Vector3& _scale,
			const DXSimp::Quaternion& _rotation,
			float _alpha
		);
	};
}