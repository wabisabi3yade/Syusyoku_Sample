#include "pch.h"
#include "AssetPath_Base.h"

namespace HashiTaku
{
	void AssetPath_Base::SetPathName(const std::string& _pathName)
	{
		pathName = _pathName;
	}

	std::string AssetPath_Base::GetPathName() const
	{
		return pathName;
	}

	json AssetPath_Base::Save()
	{
		auto data = Asset_Base::Save();

		data["pathName"] = pathName;

		return data;
	}

	void AssetPath_Base::Load(const json& _data)
	{
		Asset_Base::Load(_data);

		LoadJsonString("pathName", pathName, _data);
	}
}