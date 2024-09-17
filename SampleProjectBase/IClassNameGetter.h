#pragma once

/// @brief クラス名を取得するインターフェース
class IClassNameGetter
{
public:
	/// @brief クラス名を返す
	/// @return クラス名
	virtual std::string ClassNameToStr() = 0;
};

