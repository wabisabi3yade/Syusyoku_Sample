#include "pch.h"
#include "SaveJson.h"

namespace HashiTaku
{
	void SaveJsonVector2(const std::string& _s, const DirectX::SimpleMath::Vector2& _v,
		nlohmann::json& _j)
	{
		_j[_s][0] = _v.x;
		_j[_s][1] = _v.y;
	}

	void SaveJsonVector3(const std::string& _s, const DirectX::SimpleMath::Vector3& _v, nlohmann::json& _j)
	{
		_j[_s][0] = _v.x;
		_j[_s][1] = _v.y;
		_j[_s][2] = _v.z;
	}

	void SaveJsonVector4(const std::string& _s, const DirectX::SimpleMath::Vector4& _v, nlohmann::json& _j)
	{
		_j[_s][0] = _v.x;
		_j[_s][1] = _v.y;
		_j[_s][2] = _v.z;
		_j[_s][3] = _v.w;
	}

	bool LoadJsonData(const std::string& _s, nlohmann::json& _d, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_object())
		{
			return false;
		}
#endif

		_d = _j[_s];
		return true;
	}

	bool LoadJsonDataArray(const std::string& _s, nlohmann::json& _d, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array() && !_j[_s].is_object())
		{
			return false;
		}
#endif

		_d = _j[_s];
		return true;
	}

	bool LoadJsonBoolean(const std::string& _s, bool& _b, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_boolean())
		{
			return false;
		}
#endif

		_b = _j[_s];
		return true;
	}

	bool LoadJsonInteger(const std::string& _s, int& _i, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_number_integer())
		{
			return false;
		}
#endif

		_i = _j[_s];
		return true;
	}

	bool LoadJsonUnsigned(const std::string& _s, u_int& _u, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_number_unsigned())
		{
			return false;
		}
#endif
		_u = _j[_s];
		return true;
	}

	bool LoadJsonFloat(const std::string& _s, float& _f, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_number_float())
		{
			return false;
		}
#endif
		_f = _j[_s];
		return true;
	}

	bool LoadJsonString(const std::string& _s, std::string& _str, const nlohmann::json& _j)
	{
		if (!IsJsonContains(_j, _s)) return false;

#ifdef EDIT
		if (!_j[_s].is_string())
		{
			return false;
		}
#endif
		_str = _j[_s].get<std::string>();
		return true;
	}

	bool LoadJsonVector2(const std::string& _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array())
		{
			return false;
		}
#endif
		_v.x = _j[_s][0];
		_v.y = _j[_s][1];
		return true;
	}

	bool LoadJsonVector3(const std::string& _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array())
		{
			return false;
		}
#endif
		_v.x = _j[_s][0];
		_v.y = _j[_s][1];
		_v.z = _j[_s][2];
		return true;
	}

	bool LoadJsonVector4(const std::string& _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array())
		{
			return false;
		}
#endif
		_v.x = _j[_s][0];
		_v.y = _j[_s][1];
		_v.z = _j[_s][2];
		_v.w = _j[_s][3];
		return true;
	}

	bool LoadJsonQuaternion(const std::string& _s, DirectX::SimpleMath::Quaternion& _v, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array())
		{
			return false;
		}
#endif
		_v.x = _j[_s][0];
		_v.y = _j[_s][1];
		_v.z = _j[_s][2];
		_v.w = _j[_s][3];
		return true;
	}

	bool LoadJsonColor(const std::string& _s, DirectX::SimpleMath::Color& _c, const nlohmann::json& _j)
	{

		if (!IsJsonContains(_j, _s)) return false;
#ifdef EDIT
		if (!_j[_s].is_array())
		{
			return false;
		}
#endif
		_c.x = _j[_s][0];
		_c.y = _j[_s][1];
		_c.z = _j[_s][2];
		_c.w = _j[_s][3];
		return true;
	}

	bool IsJsonContains(const nlohmann::json& _j, const std::string& _elementStr)
	{
		if (!_j.contains(_elementStr))
		{
			return false;
		}
		return true;
	}
}