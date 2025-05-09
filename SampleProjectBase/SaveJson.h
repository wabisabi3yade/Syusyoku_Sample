#pragma once

namespace HashiTaku
{
	// セーブ
	void SaveJsonVector2(const std::string& _s, const DXSimp::Vector2& _v,
		json& _j);
	void SaveJsonVector3(const std::string& _s, const DXSimp::Vector3& _v,
		json& _j);
	void SaveJsonVector4(const std::string& _s, const DXSimp::Vector4& _v,
		json& _j);

	// ロード 戻り値：ロード成功か？

	bool LoadJsonData(const std::string& _s, json& _d, const json& _j);
	bool LoadJsonDataArray(const std::string& _s, json& _d, const json& _j);
	bool LoadJsonBoolean(const std::string& _s, bool& _b, const json& _j);
	bool LoadJsonInteger(const std::string& _s, int& _i, const json& _j);
	bool LoadJsonUnsigned(const std::string& _s, u_int& _u, const json& _j);
	bool LoadJsonFloat(const std::string& _s, float& _f, const json& _j);
	bool LoadJsonString(const std::string& _s, std::string& _str, const json& _j);
	bool LoadJsonVector2(const std::string& _s, DXSimp::Vector2& _v, const json& _j);
	bool LoadJsonVector3(const std::string& _s, DXSimp::Vector3& _v, const json& _j);
	bool LoadJsonVector4(const std::string& _s, DXSimp::Vector4& _v, const json& _j);
	bool LoadJsonQuaternion(const std::string& _s, DXSimp::Quaternion& _v, const json& _j);
	bool LoadJsonColor(const std::string& _s, DXSimp::Color& _c, const json& _j);
	template<typename T> bool LoadJsonEnum(const std::string& _s, T& _e, const json& _j);

	/// @brief jsonデータにあるか確認する
	/// @param _j jsonデータ
	/// @param _elementStr 指定する文字列
	/// @return 含まれているか？
	bool IsJsonContains(const json& _j, const std::string& _elementStr);

	template<typename T>
	bool LoadJsonEnum(const std::string& _s, T& _e, const json& _j)
	{
		if (!IsJsonContains(_j, _s))
			return false;
#ifdef EDIT
		if (!_j[_s].is_number_integer())
		{
			HASHI_DEBUG_LOG("は列挙型ではありません");
			return false;
		}
#endif // EDIT

		_e = static_cast<T>(_j[_s]);
		return true;
	}
}