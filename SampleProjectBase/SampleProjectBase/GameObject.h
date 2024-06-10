#pragma once

// シーンで使用するオブジェクト全般の基底クラス
class GameObject
{
protected:
	bool isActive;	// アクティブ状態かどうか

public:
	Transform transform;	// Transformパラメータ

	GameObject() : isActive(true) {};
	virtual ~GameObject() {};

	virtual void Update(){};	// 更新処理
	virtual void LateUpdate() {};	// Updateを行ったあとの更新処理
	virtual void Draw() {};	// 描画処理
};

