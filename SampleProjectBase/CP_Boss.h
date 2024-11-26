#pragma once
#include "CP_Enemy.h"
#include "CP_Weapon.h"
#include "BossActionController.h"


/// @brief ボスのコンポーネント
class CP_Boss : public CP_Enemy
{
	/// @brief 武器名
	std::string weaponObjName;

	/// @brief アクション管理
	std::unique_ptr<BossActionController> pActionController;

	/// @brief アニメーション
	CP_Animation* pAnimation;

	/// @brief 武器オブジェクト
	CP_Weapon* pWeapon;

	/// @brief 攻撃判定
	const bool* pCanAttack;
public:
	CP_Boss();
	~CP_Boss() {}

	/// @brief 初期処理
	void Init() override;

	/// @brief 攻撃情報をセット
	/// @param _attackInfo 攻撃情報
	void SetAttackInfo(const HashiTaku::AttackInformation& _attackInfo);

	/// @param _data セーブシーンデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするシーンデータ 
	void Load(const nlohmann::json& _data) override;
private:
	void Awake() override;
	void Start() override;
	void Update() override;
	void Draw() override;

	/// @brief 攻撃できるか更新する
	void UpdateAttackCollision();

	/// @brief 武器をセットする
	void SetupWeapon();

	/// @brief 更新できるか？
	/// @return 更新できるか？
	bool GetCanUpdate();

	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;

	void ImGuiDebug() override;
};

