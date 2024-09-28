#pragma once

namespace HashiTaku
{
	// �Z�[�u
	void SaveJsonVector2(const std::string& _s, const DirectX::SimpleMath::Vector2& _v,
		nlohmann::json& _j);
	void SaveJsonVector3(const std::string& _s, const DirectX::SimpleMath::Vector3& _v,
		nlohmann::json& _j);
	void SaveJsonVector4(const std::string& _s, const DirectX::SimpleMath::Vector4& _v,
		nlohmann::json& _j);

	// ���[�h �߂�l�F���[�h�������H

	bool LoadJsonData(const std::string& _s, nlohmann::json& _d, const nlohmann::json& _j);
	bool LoadJsonDataArray(const std::string& _s, nlohmann::json& _d, const nlohmann::json& _j);
	bool LoadJsonBoolean(const std::string& _s, bool& _b, const nlohmann::json& _j);
	bool LoadJsonInteger(const std::string& _s, int& _i, const nlohmann::json& _j);
	bool LoadJsonUnsigned(const std::string& _s, u_int& _u, const nlohmann::json& _j);
	bool LoadJsonFloat(const std::string& _s, float& _f, const nlohmann::json& _j);
	bool LoadJsonString(const std::string& _s, std::string& _str, const nlohmann::json& _j);
	bool LoadJsonVector2(const std::string& _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j);
	bool LoadJsonVector3(const std::string& _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j);
	bool LoadJsonVector4(const std::string& _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j);
	bool LoadJsonQuaternion(const std::string& _s, DirectX::SimpleMath::Quaternion& _v, const nlohmann::json& _j);
	bool LoadJsonColor(const std::string& _s, DirectX::SimpleMath::Color& _c, const nlohmann::json& _j);
	template<typename T> bool LoadJsonEnum(const std::string& _s, T& _e, const nlohmann::json& _j);

	/// @brief json�f�[�^�ɂ��邩�m�F����
	/// @param _j json�f�[�^
	/// @param _elementStr �w�肷�镶����
	/// @return �܂܂�Ă��邩�H
	bool IsJsonContains(const nlohmann::json& _j, const std::string& _elementStr);

	template<typename T>
	bool LoadJsonEnum(const std::string& _s, T& _e, const nlohmann::json& _j)
	{
		if (!IsJsonContains(_j, _s))
			return false;

		if (!_j[_s].is_number_integer())
		{
			HASHI_DEBUG_LOG("�͗񋓌^�ł͂���܂���");
			return false;
		}

		_e = static_cast<T>(_j[_s]);
		return true;
	}
}