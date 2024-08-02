#pragma once

// アセットの基底クラス
class Asset_Base
{
protected:

	/// @brief アセットの名前
	std::string name;
public:

	/// @brief コンストラクタ
	/// @param _type アセットの種類
	Asset_Base() : name("") {};
	virtual ~Asset_Base() {};

	/// @brief 名前をセットする
	/// @param _name アセットの名前
	void SetAssetName(const std::string& _name);

	/// @brief 名前を取得する
	/// @return アセットの名前
	std::string GetAssetName()const;
};