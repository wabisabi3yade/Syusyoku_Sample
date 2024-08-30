#pragma once
#include "ISaveLoad.h"

class GameObject;

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component : public ISaveLoad, public HashiTaku::IImGuiUser
{
	friend class GameObject;
	
	// 名前
	std::string name;

	// 活動状態
	bool isEnable;

	/// @brief Awake処理を行ったかどうか？
	bool isAlreadyAwake;

	/// @brief Start処理を行ったかどうか？
	bool isAlreadyStart;
protected:

	// このコンポーネントの所持オブジェクト
	GameObject* gameObject;
public:
	Component() : name(""), isEnable(true), isAlreadyAwake(false), isAlreadyStart(false), gameObject(nullptr) {}
	virtual ~Component(){};

	virtual void Init() {};

	/// @brief 外部からで呼び出すAwake処理
	void AwakeBase();

	/// @brief 外部からで呼び出すStart処理
	void StartBase();

	// 毎フレーム更新処理
	virtual void Update() {};	

	// Updateの後に行う更新処理
	virtual void LateUpdate() {};

	// 描画処理
	virtual void Draw() {};

	/// @brief 活動状態を切り替える
	void TransitionEnable();

	/// @brief 名前をセットする
	/// @param _name 名前
	void SetName(const std::string& _name);

	// 活動状態をセット
	void SetEnable(bool _enable);

	// 名前を取得
	std::string GetName() const;

	// オブジェクトのトランスフォーム
	Transform& GetTransform() const;

	// ゲームオブジェクト取得	
	GameObject& GetGameObject() const { return *gameObject; }

	// 活動フラグ取得
	bool GetIsEnable() const;

	// Awake処理行ったか取得
	bool GetIsAlreadyAwake() const;

	// Start処理行ったか取得
	bool GetIsAlreadyStart() const;

	/// @brief セーブする
	/// @param _sceneData セーブシーンデータ
	virtual nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _sceneData ロードするシーンデータ 
	virtual void Load(const nlohmann::json& _sceneData) override;
	
protected:
	/// @brief 作成時に呼ばれる処理
	virtual void Awake() {}

	// 1フレーム目で行う処理
	virtual void Start() {};

	// ImGuiでパラメータを変える処理
	void ImGuiSetting() override {};
};
