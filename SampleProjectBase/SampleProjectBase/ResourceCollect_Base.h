#pragma once

#include "Texture.h"
#include "Material.h"
#include "Model.h"

/// <summary>
/// リソースを管理する基底クラス
/// </summary>
/// <typeparam name="T">管理するリソーｓの型</typeparam>
template <typename T>
class ResourceCollect_Base
{
protected:
	virtual ~ResourceCollect_Base();
	// 使われているモデルの配列リスト
	std::unordered_map<std::string, std::unique_ptr<T>> resourceList;

public:
	/// <summary>
	/// 引数の名前のリソースがあるか確認する
	/// </summary>
	/// <param name="_checkName"></param>
	/// <returns></returns>
	bool GetIsImported(std::string _checkName);

	bool SetResource(std::unique_ptr<T> _pResource, std::string _setName);

	/// <summary>
	/// リソースのconstポインタを取得する
	/// </summary>
	/// <param name="_getName">取得する名前</param>
	/// <returns>指定したconstポインタ</returns>
	const T* GetConstResource(std::string _getName);

	/// <summary>
	/// リソースの生ポインタを取得するtypename
	/// </summary>
	/// <param name="_getName">取得する名前</param>
	/// <returns>指定したポインタ</returns>
	T* GetResource(std::string _getName);
};

template<typename T>
inline ResourceCollect_Base<T>::~ResourceCollect_Base()
{

}

template<typename T>
inline bool ResourceCollect_Base<T>::GetIsImported(std::string _checkName)
{
	// 名前を探索する
	auto itr = resourceList.find(_checkName);
	// リスト内に入ってるなら
	if (itr != resourceList.end())
	{
		return true;
	}

	return false;
}

template<typename T>
inline bool ResourceCollect_Base<T>::SetResource(std::unique_ptr<T> _pResource, std::string _setName)
{
	// リソースの名前がすでにあるなら
	if (GetIsImported(_setName))
	{
		std::string message = "既にリソースがあるのでセットできません" + _setName;
		ImGuiDebugLog::AddDebugLog(message);
		return false;
	}
	// リソースはnullptrなら
	else if (_pResource == nullptr)
	{
		std::string message = "リソースがnullptrです" + _setName;
		ImGuiDebugLog::AddDebugLog(message);
		return false;
	}

	// 配列に確保する
	resourceList.emplace(_setName, std::move(_pResource));
	return true;
}

template<typename T>
inline const T* ResourceCollect_Base<T>::GetConstResource(std::string _getName)
{
	// モデルがロードされていないなら
	if (!GetIsImported(_getName))
	{
		std::string message = "ロードされていないリソースです　" + _getName;
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	return resourceList[_getName].get();
}

template<typename T>
inline T* ResourceCollect_Base<T>::GetResource(std::string _getName)
{
	// モデルがロードされていないなら
	if (!GetIsImported(_getName))
	{
		std::string message = "ロードされていないリソースです　" + _getName;
		ImGuiDebugLog::AddDebugLog(message);
		return nullptr;
	}

	return resourceList[_getName].get();
}
