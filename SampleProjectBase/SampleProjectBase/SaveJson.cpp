#include "pch.h"
#include "SaveJson.h"

bool SaveJson::SaveObject(GameObject& _saveObject)
{
	

	return false;
}

void SaveJson::LoadObject(GameObject& _gameObject, const nlohmann::json& _j)
{
}

void DirectX::SimpleMath::SaveJsonVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v, nlohmann::json& _j)
{
	std::string set = _s + JsonAddX();
	_j[set] = _v.x;
	set = _s + JsonAddY();
	_j[set] = _v.y;
}

void DirectX::SimpleMath::SaveJsonVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v, nlohmann::json& _j)
{
	std::string set = _s + JsonAddX();
	_j[set] = _v.x;
	set = _s + JsonAddY();
	_j[set] = _v.y;
	set = _s + JsonAddZ();
	_j[set] = _v.z;
}

void DirectX::SimpleMath::SaveJsonVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v, nlohmann::json& _j)
{
	std::string set = _s + JsonAddX();
	_j[set] = _v.x;
	set = _s + JsonAddY();
	_j[set] = _v.y;
	set = _s + JsonAddZ();
	_j[set] = _v.z;
	set = _s + JsonAddW();
	_j[set] = _v.w;
}

void DirectX::SimpleMath::LoadVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j)
{
}

void DirectX::SimpleMath::LoadVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j)
{
}

void DirectX::SimpleMath::LoadVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j)
{
}
