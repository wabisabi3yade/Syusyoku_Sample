#pragma once

namespace HashiTaku
{
	class Sprite;
	class Material;

	/// @brief �X�v���C�g��`�悷��@�\
	class SpriteDrawer
	{

	public:
		SpriteDrawer() {}
		~SpriteDrawer() {}

		/// @brief �X�v���C�g�`��
		/// @param _sprite �X�v���C�g
		/// @param _material �}�e���A��
		/// @param _pos ���[���h���W
		/// @param _scale ���[���h�X�P�[��
		/// @param _rotation ���[���h��]��
		/// @param _alpha ���l
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