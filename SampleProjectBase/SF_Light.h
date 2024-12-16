#pragma once

namespace HashiTaku
{
	class DirectionLight;
	class PointLight;
	class SpotLight;

	// �V�[�����֐�
	namespace SceneFunction
	{
		// ���C�g
		namespace LightFunc
		{
			/// @brief �f�B���N�V�������C�g�쐬
			/// @return �쐬�����f�B���N�V�������C�g
			DirectionLight* CreateDirection();

			/// @brief �|�C���g���C�g�쐬
			/// @return �|�C���g���C�g
			PointLight* CreatePoint();

			/// @brief �X�|�b�g���C�g�쐬
			/// @return �쐬�����X�|�b�g���C�g
			SpotLight* CreateSpot();
		}

	}
}