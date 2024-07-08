#pragma once

class GameObject;
// ゲームオブジェクト関連
namespace SceneFunction
{
	// オブジェクトを作成する
	GameObject& CreateObject(const GameObject& _obj, DirectX::SimpleMath::Vector3 _worldPos);
}

