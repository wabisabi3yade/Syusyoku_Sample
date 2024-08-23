#pragma once

namespace HashiTaku
{
	// セーブ
	void SaveJsonVector2(const std::string& _s, const DirectX::SimpleMath::Vector2& _v,
		nlohmann::json& _j);
	void SaveJsonVector3(const std::string& _s, const DirectX::SimpleMath::Vector3& _v,
		nlohmann::json& _j);
	void SaveJsonVector4(const std::string& _s, const DirectX::SimpleMath::Vector4& _v,
		nlohmann::json& _j);

	// ロード
	void LoadJsonBoolean(const std::string& _s, bool& _b, const nlohmann::json& _j);
	void LoadJsonInteger(const std::string& _s, int& _i, const nlohmann::json& _j);
	void LoadJsonUnsigned(const std::string& _s, u_int& _u, const nlohmann::json& _j);
	void LoadJsonFloat(const std::string& _s, float& _f, const nlohmann::json& _j);
	void LoadJsonString(const std::string& _s, std::string& _str, const nlohmann::json& _j);
	void LoadJsonVector2(const std::string& _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j);
	void LoadJsonVector3(const std::string& _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j);
	void LoadJsonVector4(const std::string& _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j);
	void LoadJsonColor(const std::string& _s, DirectX::SimpleMath::Color& _c, const nlohmann::json& _j);
	template<typename T> void LoadJsonEnum(const std::string& _s, T& _e, const nlohmann::json& _j);

	/// @brief jsonデータにあるか確認する
	/// @param _j jsonデータ
	/// @param _elementStr 指定する文字列
	/// @return 含まれているか？
	bool IsJsonContains(const nlohmann::json& _j, const std::string& _elementStr);


	template<typename T>
	void LoadJsonEnum(const std::string& _s, T& _e, const nlohmann::json& _j)
	{
		if (!IsJsonContains(_j, _s))
			return;

		if (!_j[_s].is_number_integer())
		{
			HASHI_DEBUG_LOG(_s + "は列挙型ではありません");
			return;
		}

		_e = static_cast<T>(_j[_s]);
	}
}