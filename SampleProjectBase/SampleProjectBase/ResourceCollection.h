#pragma once
#include "Singleton_Base.h"
class Resource_Base;

// ゲームないでの利用するリソース(テクスチャ、モデルなど)をunique_ptrで管理するクラス
class ResourceCollection :
	public Singleton_Base<ResourceCollection>
{
	friend class Singleton_Base<ResourceCollection>;
	std::unordered_map<std::string, std::unique_ptr<Resource_Base>> resources;	// 名前がキー値のリソース配列
public:
	/// <summary>
	/// 名前のリソースが既に存在してるか確認する
	/// </summary>
	/// <param name="_resourceName">確認するリソース名</param>
	/// <returns>既に入っているフラグ</returns>
	bool GetImpotred(std::string _resourceName);

	/// <summary>
	/// リソース名とリソースのポインタを配列に追加する
	/// </summary>
	/// <typeparam name="T">そのリソースのポインタクラス</typeparam>
	/// <param name="_resourceName">追加する名前</param>
	/// <param name="_resourcePtr">追加するポインタ</param>
	/// <returns>追加したリソースの生ポインタ</returns>
	template<class T> T* SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr);

	/// <summary>
	/// 名前に対応したリソースポインタを取得する
	/// </summary>
	/// <typeparam name="T">そのリソースのポインタクラス</typeparam>
	/// <param name="_resourceName">リソースの名前</param>
	/// <returns>リソースの生ポインタ</returns>
	template<class T> T* GetResource(std::string _resourceName);
};

// リソース基底クラス
class Resource_Base
{
public:
	Resource_Base() {};
	virtual ~Resource_Base() {};
};

// リソースクラス
template<class T>
class Resource : Resource_Base
{
	std::unique_ptr<T> ptr;	// ポインタクラス
public:
	Resource(std::unique_ptr<T> _ptr) : ptr(_ptr) {};
	~Resource() {};

	T* GetPtr() { return ptr.get(); }	// 生ポインタ取得
};

template<class T>
inline T* ResourceCollection::SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr)
{
	// デバッグ時にだけ名前参照をして被るか確認
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())	// 既に名前があったら
	{
		std::string message = "既にリソースがあります" + _resourceName;
		MessageError(message.c_str());
		return nullptr;
	}

	// リソースを生成し、配列に入れる
	std::unique_ptr<Resource> resource = std::make_unique<Resource>(std::move(_resourcePtr));
	resources.emplace(_resourceName, resource);
	
	// リソースポインタを返す
	return resource.GetPtr();
}

template<class T>
inline T* ResourceCollection::GetResource(std::string _resourceName)
{
	// 名前から探す
	auto itr = resources.find(_resourceName);
	if (itr != resources.end())
	{
		std::string message = "ロードされていないので取得できません　" + _resourceName;
		MessageError(message.c_str());
		return nullptr;
	} 

	// 生ポインタをもらう
	Resource_Base* basePtr = itr->second.get();
	// Resourceクラスにキャスト変換し、リソースのポインタをもらう
	Resource* resource = dynamic_cast<Resource*>(basePtr);
	T* retPtr = resource->GetPtr();

	return retPtr;	// リソースのポインタを返す
}
