#pragma once
#include "GameObject.h"

// シーン内関数
namespace SceneFunction
{
	// オブジェクト
	class ObjectFunc
	{
		/// @brief シーンオブジェクトに追加する
		/// @param _pAddObject 追加するオブジェクト
		/// @return 追加したオブジェクトを返す
		static GameObject& AddSceneObject(std::unique_ptr<GameObject> _pAddObject);

	public:
		/// @brief オブジェクトを新しく生成する
		/// @param _obj 対象オブジェクト
		/// @param _worldPos 生成する座標
		/// @return 
		static GameObject& Instantiate(const GameObject& _obj, const DirectX::SimpleMath::Vector3& _worldPos);

		/// @brief 空オブジェクトを作成し、シーンオブジェクトに追加
		/// @param _objectName オブジェクト名
		/// @return 空オブジェクト
		static GameObject& CreateEmpty(const std::string& _objectName);

		/// @brief オブジェクトを削除する
		/// @param _deleteName 削除する名前
		static void DeleteObject(const std::string& _deleteName);
	};

}

