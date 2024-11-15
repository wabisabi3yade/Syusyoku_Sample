#pragma once
#include "CP_Character.h"
#include "PlayerActionController.h"
#include "AttackInformation.h"

class CP_Weapon;

class CP_Player : public CP_Character
{
	/// @brief アクションコントローラー
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief アニメーション
	CP_Animation* pAnimation;

	/// @brief 武器判定
	CP_Weapon* pWeapon;

	/// @brief 攻撃フラグ
	const bool* pAttackCollisionFlag;

	/// @brief ヒットストップする前の再生速度
	float hitStopBeforeAnimSpeed;
public:
	CP_Player();
	~CP_Player() {}

	// コンポーネント共通
	void Init() override;

	/// @brief ヒットストップ開始した時の処理
	void OnHitStopBegin() override;

	/// @brief ヒットストップ終了した時の処理
	void OnHitStopEnd() override;

	/// @brief 武器コンポーネントをセット
	/// @param _setWeapon 武器コンポーネント
	void SetWeapon(CP_Weapon& _setWeapon);

	/// @brief 攻撃情報をセットする
	/// @param _setAttackInfo 攻撃情報
	void SetAttackInfo(const HashiTaku::AttackInformation& _setAttackInfo);

	/// @brief 攻撃フラグを取得
	/// @return 攻撃フラグ
	bool GetIsAttackFlag() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;

private:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;

	/// @brief 武器の攻撃フラグをセット
	void SetWeaponAttackFlag();

	/// @brief 更新できるか取得する
	/// @return 更新できるか？
	bool GetCanUpdate() const;

	/// @brief プレイヤーのダメージ処理
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;
	void OnDeathBehavior() override;	

	void ImGuiDebug() override;
	void ImGuiSetWeapon();
};


