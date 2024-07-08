#pragma once
#include "GameObject.h"


// jsonファイルにパラメータをセーブする
class SaveJson
{
	
public:
	/// <summary>
	/// オブジェクトをセーブする
	/// </summary>
	/// <param name="_saveObject">セーブするオブジェクト</param>
	/// <returns>成功フラグ</returns>
	static bool SaveObject(GameObject& _saveObject);

	// ゲームオブジェクトをJsonファイルから読み込む
	static void LoadObject(GameObject& _gameObject, const nlohmann::json& _j);
};


namespace DirectX::SimpleMath
{
	// Vector型をそれぞれメンバ変数で保存するときに名前の後ろにつける名前
	constexpr std::string JsonAddX() { return "_x"; }
	constexpr std::string JsonAddY() { return "_y"; }
	constexpr std::string JsonAddZ() { return "_z"; }
	constexpr std::string JsonAddW() { return "_w"; }

	// Vector型保存
	static void SaveJsonVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v,
		nlohmann::json& _j);
	static void SaveJsonVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v,
		nlohmann::json& _j);
	static void SaveJsonVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v,
		nlohmann::json& _j);
	// Vector型読込
	static void LoadVector2(const std::string _s, DirectX::SimpleMath::Vector2& _v, const nlohmann::json& _j);
	static void LoadVector3(const std::string _s, DirectX::SimpleMath::Vector3& _v, const nlohmann::json& _j);
	static void LoadVector4(const std::string _s, DirectX::SimpleMath::Vector4& _v, const nlohmann::json& _j);

}