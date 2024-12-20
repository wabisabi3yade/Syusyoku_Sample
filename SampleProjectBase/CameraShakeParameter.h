#pragma once

namespace HashiTaku
{
	/// @brief レベルごとに設定するパラメータ
	class CameraShakeParameter : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief 揺れる方向ベクトル
		DXSimp::Vector3 shakeVec;

		/// @brief 揺れの力
		float power{ 1.0f };

		/// @brief 揺らす時間
		float time{ 0.3f };

		/// @brief 揺らす速度
		float speed{ 1.0f };

		/// @brief フェードアウトするか？
		bool isFadeOut{ false };

		CameraShakeParameter() = default;
		~CameraShakeParameter() = default;

		nlohmann::json Save() override;
		void Load(const nlohmann::json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}


