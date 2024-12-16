#include "Asset_Base.h"

namespace HashiTaku
{
	void Asset_Base::SetAssetName(const std::string& _name)
	{
		assetName = _name;
	}

	const std::string& Asset_Base::GetAssetName() const
	{
		return assetName;
	}

	bool Asset_Base::GetIsSave() const
	{
		return isSave;
	}

	nlohmann::json Asset_Base::Save()
	{
		nlohmann::json data;
		data["assetName"] = assetName;

		return data;
	}

	void Asset_Base::Load(const nlohmann::json& _data)
	{
		LoadJsonString("assetName", assetName, _data);
	}

	void Asset_Base::SetIsntSave()
	{
		isSave = false;
	}
}