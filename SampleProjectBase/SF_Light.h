#pragma once

namespace HashiTaku
{
	class DirectionLight;
	class PointLight;
	class SpotLight;

	// シーン内関数
	namespace SceneFunction
	{
		// ライト
		namespace LightFunc
		{
			/// @brief ディレクションライト作成
			/// @return 作成したディレクションライト
			DirectionLight* CreateDirection();

			/// @brief ポイントライト作成
			/// @return ポイントライト
			PointLight* CreatePoint();

			/// @brief スポットライト作成
			/// @return 作成したスポットライト
			SpotLight* CreateSpot();
		}

	}
}