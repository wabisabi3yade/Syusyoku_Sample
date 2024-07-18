#pragma once

class GameObject;

// シーン内関数
namespace SceneFunction
{
	// オブジェクト
	namespace ObjectFunc
	{
		/// @brief オブジェクトを新しく生成する
		/// @param _obj 対象オブジェクト
		/// @param _worldPos 生成する座標
		/// @return 
		GameObject& Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos);

		/// @brief 
		/// @param _objectName 
		/// @return 
		GameObject& CreateEmpty(std::string _objectName);
	}
	
}

