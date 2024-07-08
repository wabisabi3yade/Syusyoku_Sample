#pragma once
#include "SaveJson.h"

class SaveJsonValue;

// Jsonにセーブする
namespace HashiTaku_JsonSave
{
	using namespace DirectX::SimpleMath;	// この範囲内だけ使用する
	using json =  nlohmann::json;	// この範囲内だけ使用する
	// コンセプトで配列に追加できる型名を制限する
	template<typename T>
	concept AllowType =
		std::is_same_v<T, bool> || std::is_same_v<T, short> ||
		std::is_same_v<T, u_int> || std::is_same_v<T, int> ||
		std::is_same_v<T, float> || std::is_same_v<T, double> ||
		std::is_same_v<T, std::string> || std::is_same_v<T, Vector2> ||
		std::is_same_v<T, Vector3> || std::is_same_v<T, Vector4> ||
		std::is_same_v<T, Color>;

	// テンプレートを使用していない抜け殻クラス
	class JsonValue_Base
	{
		friend class SaveJsonValue;	//  SaveJsonValueでしか定義させないようにする

	public:
		JsonValue_Base() {};
		virtual void JasonSave(json& _j){};
	};

	// 配列にテンプレートを使用する為の本体クラス
	template <HashiTaku_JsonSave::AllowType T>
	class JsonValue : private JsonValue_Base
	{
		friend class SaveJsonValue;	//  SaveJsonValueでしか定義させないようにする
		std::string& name;	// 変数名
		T& value;	// 変数の値
	public:
		JsonValue() {};
		void JasonSave(json& _j)override;	// この値をjsonにセーブする
	};

	template<HashiTaku_JsonSave::AllowType T>
	inline void JsonValue<T>::JasonSave(json& _j)
	{
		// 型名に応じた方法でjsonに書き込む
		auto ti = typeid(T);
		if (ti == typeid(Vector3))	// 一番Vector3型が使われるので上に
		{
			SaveVector3(name, value, _j);
		}
		else if (ti == typeid(Vector4) || ti == typeid(Color))	// Colorもここに
		{
			SaveVector4(name, value, _j);
		}
		else if (ti == typeid(Vector2))
		{
			SaveVector2(name, value, _j);
		}
		else
		{
			_j[name] = value;	// 直接いれる
		}
	}

}

// Jsonにセーブするための配列を持つクラス
class SaveJsonValue
{
	std::list<HashiTaku_JsonSave::JsonValue_Base> saveValues;	// セーブする変数配列

public:
	SaveJsonValue() {};
	~SaveJsonValue() { saveValues.clear(); }

	// 配列に追加する
	template<HashiTaku_JsonSave::AllowType T> void Add(const std::string& _name, const T& _saveValue);

	// 持ってる配列にJsonファイルに書き込む
	void WriteJson(nlohmann::json& _jsonData);
};

template<HashiTaku_JsonSave::AllowType T>
inline void SaveJsonValue::Add(const std::string& _name, const T& _saveValue)
{
	HashiTaku_JsonSave::JsonValue<T> jsonVal;
	jsonVal.name = _name;	// 変数名を代入
	jsonVal.value = _saveValue;	// 値を代入
	
	saveValues.push_back(jsonVal);	// 追加
}


