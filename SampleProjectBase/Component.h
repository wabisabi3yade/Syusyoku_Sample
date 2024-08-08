#pragma once
class GameObject;

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component
{
	friend class GameObject;
	
	// 活動状態
	bool isEnable;

protected:
	// 名前
	std::string name;	

	// このコンポーネントの所持オブジェクト
	GameObject* gameObject;

public:
	Component() : isEnable(true), name(""), gameObject(nullptr) {}
	virtual ~Component(){};

	// AddComponentされたときに行う
	virtual void Init() {};	

	// 毎フレーム更新処理
	virtual void Update() {};	

	// Updateの後に行う更新処理
	virtual void LateUpdate() {};

	// 描画処理
	virtual void Draw() {};

	// ImGuiでパラメータを変える処理
	virtual void ImGuiSetting() {};

	/// @brief 活動状態を切り替える
	void TransitionEnable();

	// 名前を取得
	std::string GetName() const;

	// オブジェクトのトランスフォーム
	Transform& GetTransform() const;

	// ゲームオブジェクト取得	
	const GameObject& GetGameObject() const { return *gameObject; }

	bool GetIsEnable() const;
};