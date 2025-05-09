#pragma once

namespace HashiTaku
{
	/// @brief パーリンノイズシェイクするときのパラメータ
	class PerlinShakeParameter : public IImGuiUser, public ISaveLoad
	{
	public:
		/// @brief 揺れる方向ベクトル
		DXSimp::Vector3 shakeVec;

		/// @brief 揺れの力
		float power;

		/// @brief 最低限に揺らすパワー
		float minPowerRatio;

		/// @brief 揺らす時間
		float time;

		/// @brief 揺らす速度
		float speed;

		/// @brief フェードアウトするか？
		bool isFadeOut;

		PerlinShakeParameter();
		~PerlinShakeParameter() = default;

		json Save() override;
		void Load(const json& _data) override;
	private:
		void ImGuiDebug() override;
	};
}


