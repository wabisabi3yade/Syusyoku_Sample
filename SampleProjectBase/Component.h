#pragma once
#include "ISaveLoad.h"

class GameObject;

// ゲームのサブシステムとなるコンポーネントの基底クラス
class Component : public ISaveLoad, public HashiTaku::IImGuiUser
{
	friend class GameObject;
	
	/// @brief 名前
	std::string name;

	/// @brief Awake処理を行ったかどうか？
	bool isAlreadyAwake;

	/// @brief Start処理を行ったかどうか？
	bool isAlreadyStart;
protected:
	/// @brief 活動状態
	bool isEnable;

	/// @brief このコンポーネントの所持オブジェクト
	GameObject* gameObject;
public:
	Component() : name(""), isEnable(true), isAlreadyAwake(false), isAlreadyStart(false), gameObject(nullptr) {}
	virtual ~Component(){};

	virtual void Init() {};

	/// @brief 外部から呼び出すAwake処理
	void AwakeCall();

	/// @brief 外部から呼び出すStart処理
	void StartCall();

	/// @brief 外部から呼び出すUpdate処理
	void UpdateCall();

	/// @brief 外部から呼び出すLateUpdate処理
	void LateUpdateCall();

	/// @brief 外部から呼び出すDraw処理
	void DrawCall();

	/// @brief 活動状態を切り替える
	void TransitionEnable();

	/// @brief 削除されたときに処理する
	virtual void OnDestroy() {}

	/// @brief トランスフォームのパラメータが変更した時の処理
	virtual void OnChangeTransform() {}

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
	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
	
private:
	/// @brief 活動状態をtrueに変更したときの処理
	void OnEnableTrueCall();

	/// @brief 活動状態をfalseに変更したときの処理
	void OnEnableFalseCall();
protected:
	/// @brief 作成時に呼ばれる処理
	virtual void Awake() {}

	/// @brief 1フレーム目で行う処理
	virtual void Start() {};

	/// @brief 毎フレーム更新処理
	virtual void Update() {};

	/// @brief Updateの後に行う更新処理
	virtual void LateUpdate() {};

	/// @brief 描画処理
	virtual void Draw() {};

	/// @brief ImGuiでパラメータを変える処理
	void ImGuiSetting() override {};

	// 活動状態を切り替える時の処理
	virtual void OnEnableTrue() {}
	virtual void OnEnableFalse() {}

	/// @brief ゲームオブジェクトとコンポーネントの活動状態を取得
	/// @return どちらも活動しているか？
	bool GetIsActive();
};
