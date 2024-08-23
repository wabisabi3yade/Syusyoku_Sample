#include "pch.h"
#include "SaveJson.h"

void HashiTaku::SaveJsonVector2(const std::string& _s, const DirectX::SimpleMath::Vector2& _v,
	nlohmann::json& _j)
{
	_j[_s][0] = _v.x;
	_j[_s][1] = _v.y;
}

void HashiTaku::SaveJsonVector3(const std::string& _s, const DirectX::SimpleMath::Vector3& _v, nlohmann::json& _j)
{
	_j[_s][0] = _v.x;
	_j[_s][1] = _v.y;
	_j[_s][2] = _v.z;
}

void HashiTaku::SaveJsonVector4(const std::string& _s, const DirectX::SimpleMath::Vector4& _v, nlohmann::json& _j)
{
	_j[_s][0] = _v.x;
	_j[_s][1] = _v.y;
	_j[_s][2] = _v.z;
	_j[_s][3] = _v.w;
}

void HashiTaku::LoadJsonBoolean(const std::string& _s, bool& _b, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s)) return;

	if (!_j[_s].is_boolean())
	{
		HASHI_DEBUG_LOG(_s + "はboolではありません");
		return;
	}

	_b = _j[_s];
}

void HashiTaku::LoadJsonInteger(const std::string& _s, int& _i, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s)) return;

	if (!_j[_s].is_number_integer())
	{
		HASHI_DEBUG_LOG(_s + "はintではありません");
		return;
	}

	_i = _j[_s];
}

void HashiTaku::LoadJsonUnsigned(const std::string& _s, u_int& _u, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s)) return;

	if (!_j[_s].is_number_unsigned())
	{
		HASHI_DEBUG_LOG(_s + "はu_intではありません");
		return;
	}

	_u = _j[_s];
}

void HashiTaku::LoadJsonFloat(const std::string& _s, float& _f, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s)) return;

	if (!_j[_s].is_number_float())
	{
		HASHI_DEBUG_LOG(_s + "はfloatではありません");
		return;
	}

	_f = _j[_s];
}

void HashiTaku::LoadJsonString(const std::string& _s, std::string& _str, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s)) return;

	if (!_j[_s].is_string())
	{
		HASHI_DEBUG_LOG(_s + "はstringではありません");
		return;
	}

	_str = _j[_s].get<std::string>();
}

void HashiTaku::LoadJsonVector2(const std::string& _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s))
		return;

	if (!_j[_s].is_array())
	{
		HASHI_DEBUG_LOG(_s + "は配列ではありません");
		return;
	}

	_v.x = _j[_s][0];
	_v.y = _j[_s][1];
}

void HashiTaku::LoadJsonVector3(const std::string& _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s))
		return;

	if (!_j[_s].is_array())
	{
		HASHI_DEBUG_LOG(_s + "は配列ではありません");
		return;
	}

	_v.x = _j[_s][0];
	_v.y = _j[_s][1];
	_v.z = _j[_s][2];
}

void HashiTaku::LoadJsonVector4(const std::string& _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s))
		return;

	if (!_j[_s].is_array())
	{
		HASHI_DEBUG_LOG(_s + "は配列ではありません");
		return;
	}

	_v.x = _j[_s][0];
	_v.y = _j[_s][1];
	_v.z = _j[_s][2];
	_v.w = _j[_s][3];
}

void HashiTaku::LoadJsonColor(const std::string& _s, DirectX::SimpleMath::Color& _c, const nlohmann::json& _j)
{
	if (!IsJsonContains(_j, _s))
		return;

	if (!_j[_s].is_array())
	{
		HASHI_DEBUG_LOG(_s + "は配列ではありません");
		return;
	}

	_c.x = _j[_s][0];
	_c.y = _j[_s][1];
	_c.z = _j[_s][2];
	_c.w = _j[_s][3];
}

bool HashiTaku::IsJsonContains(const nlohmann::json& _j, const std::string& _elementStr)
{
	if (!_j.contains(_elementStr))
	{
		HASHI_DEBUG_LOG(_elementStr + "がロードデータにありませんでした");
		return false;
	}

	return true;
}
