#pragma once
#include "Singleton_Base.h"
class Resource_Base;

/* 
	命名規則　この後に_モデル名を入力(UE5に則る)
	スケルタルメッシュ：SK
	スタティックメッシュ：SM
	テクスチャ：T
	マテリアル：M
*/ 

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
	template<class T> T*  SetResource(std::string _resourceName, std::unique_ptr<T> _resourcePtr);

	/// <summary>
	/// 名前に対応したリソースポインタを取得する
	/// </summary>
	/// <typeparam name="T">そのリソースのポインタクラス</typeparam>
	/// <param name="_resourceName">リソースの名前</param>
	/// <returns>リソースの生ポインタ</returns>
	template<class T> T* GetResource(std::string _resourceName);

	/// <summary>
	/// リソースのインスタンスを作成し、配列に追加する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_resourceName"></param>
	/// <returns></returns>
	template<class T> T* CreateResource(std::string _resourceName);

	// 中身を表示する
	void ImportDisplay();
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
class Resource : public Resource_Base
{
	std::unique_ptr<T> ptr;	// ポインタクラス
public:
	// リソースポインタの所有権をこのクラスのポインタに移行させる
	Resource(std::unique_ptr<T> _ptr) { ptr = std::move(_ptr); }
	~Resource() {}

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
		ImGuiDebugLog::Add(message);
		
		// 元あるリソースのポインタを返す
		Resource<T>* resourceGetter = dynamic_cast<Resource<T>*>(itr->second.get());
		return  resourceGetter->GetPtr();
	}

	// 戻り値の生ポインタを取得
	T* retResourcePtr = _resourcePtr.get();

	// リソースを生成し、配列に入れる
	std::unique_ptr<Resource_Base> setPtr = std::make_unique<Resource<T>>(std::move(_resourcePtr));
	resources.insert(std::pair<std::string, std::unique_ptr<Resource_Base>>(_resourceName, std::move(setPtr)));
	return retResourcePtr;
}

template<class T>
inline T* ResourceCollection::GetResource(std::string _resourceName)
{
	// 名前から探す
	auto itr = resources.find(_resourceName);
	if (itr == resources.end())
	{
		std::string message = "ロードされていないので取得できません　" + _resourceName;
		ImGuiDebugLog::Add(message);
		return nullptr;
	} 

	// 生ポインタをもらう
	Resource_Base* basePtr = itr->second.get();
	// Resourceクラスにキャスト変換し、リソースのポインタをもらう
	Resource<T>* resource = dynamic_cast<Resource<T>*>(basePtr);
	T* retPtr = resource->GetPtr();

	return retPtr;	// リソースのポインタを返す
}

template<class T>
inline T* ResourceCollection::CreateResource(std::string _resourceName)
{
	// 既にその名前のリソースがあれば
	if (GetImpotred(_resourceName))
	{
		return nullptr;
	}
}
