#pragma once

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
	void DrawSprite(
		const Sprite& _sprite, 
		Material& _material, 
		const DirectX::SimpleMath::Vector3& _pos,
		const DirectX::SimpleMath::Vector3& _scale, 
		const DirectX::SimpleMath::Quaternion& _rotation
	);
};

