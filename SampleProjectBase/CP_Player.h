#pragma once
#include "CP_Character.h"
#include "PlayerActionController.h"
#include "AttackInformation.h"
#include "IUISlider.h"

class CP_Weapon;
class CP_CameraMove;

class CP_Player : public CP_Character
{
	/// @brief 武器のオブジェクト名
	std::string weaponObjName;

	/// @brief カメラオブジェクト名
	std::string cameraObjName;

	/// @brief HPスライダーオブジェクト名
	std::string hpBarObjName;

	/// @brief ガードスライダーオブジェクト名
	std::string guardBarObjName;

	/// @brief アクションコントローラー
	std::unique_ptr<PlayerActionController> pActionController;

	/// @brief アニメーション
	CP_Animation* pAnimation;

	/// @brief 武器判定
	CP_Weapon* pWeapon;

	/// @brief カメラ移動
	CP_CameraMove* pCameraMove;

	/// @brief hpゲージ
	IUISlider* pHpSlider;

	/// @brief ガードゲージ
	IUISlider* pGuardSlider;

	/// @brief 攻撃フラグ
	const bool* pAttackCollisionFlag;

	/// @brief ヒットストップする前の再生速度
	float hitStopBeforeAnimSpeed;

	/// @brief 現在のガードゲージ
	float curGuardGage;

	/// @brief 最大のガードゲージ
	float maxGuardGage;
public:
	CP_Player();
	~CP_Player() {}

	// コンポーネント共通
	void Init() override;

	/// @brief ヒットストップ開始した時の処理
	void OnHitStopBegin() override;

	/// @brief ヒットストップ終了した時の処理
	void OnHitStopEnd() override;

	/// @brief ガードゲージを増やす
	void AddGuardGage(float _addGage);

	/// @brief ガードゲージをリセット
	void ResetGuardGage();

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
	void Draw() override;
	void OnDestroy() override;

	/// @brief 更新できるか取得
	/// @return 更新できるか？
	bool CanUpdate();

	/// @brief 武器のオブジェクトをセットする
	void SetRequireObject();

	/// @brief 武器の攻撃フラグをセット
	void SetWeaponAttackFlag();

	/// @brief プレイヤーの体力をセット
	/// @param _damageVal ダメージ値
	void DecadePlayerHp(float _damageVal);

	/// @brief プレイヤーのダメージ処理
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo,
		const DirectX::SimpleMath::Vector3& _attackerPos) override;
	void OnDeathBehavior() override;	

	void ImGuiDebug() override;
	void ImGuiFindObj();
};


