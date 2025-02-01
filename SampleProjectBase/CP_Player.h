#pragma once
#include "CP_Character.h"
#include "IWeaponOwner.h"
#include "PlayerAction.h"
#include "AttackInformation.h"
#include "IUISlider.h"

namespace HashiTaku
{
	class CP_Weapon;
	class CP_CameraMove;
	class CP_StylishUI;

	class CP_Player : public CP_Character, public IWeaponOwner
	{
		/// @brief 武器のオブジェクト名
		std::string weaponObjName;

		/// @brief カメラオブジェクト名
		std::string cameraObjName;

		/// @brief HPスライダーオブジェクト名
		std::string hpBarObjName;

		/// @brief スタイリッシュUIのオブジェクト名
		std::string stylishUIName;

		/// @brief アクション変数
		std::unique_ptr<PlayerAction> pAction;

		/// @brief アニメーション
		CP_Animation* pAnimation;

		/// @brief 武器判定
		CP_Weapon* pWeapon;

		/// @brief カメラ移動
		CP_CameraMove* pCameraMove;

		/// @brief hpゲージ
		IUISlider* pHpSlider;

		/// @brief スタイリッシュUI
		CP_StylishUI* pStylishUI;

		/// @brief 攻撃フラグ
		const bool* pAttackCollisionFlag;

		/// @brief ヒットストップする前の再生速度
		float hitStopBeforeAnimSpeed;

		/// @brief 攻撃ダメージ値からスタイリッシュポイントへの倍率
		float stylishPointRatioFromAtkDmg;

		/// @brief 受けたダメージ値からスタイリッシュポイントへの倍率
		float stylishPointRatioFromAcceptDmg;

		/// @brief デバッグの無敵フラグ
		static bool isDebugInvicible;
	public:
		CP_Player();
		~CP_Player() {}

		// コンポーネント共通
		void Init() override;

		/// @brief ヒットストップ開始した時の処理
		void OnHitStopBegin() override;

		/// @brief ヒットストップ終了した時の処理
		void OnHitStopEnd() override;

		/// @brief スタイリッシュポイントを加算
		/// @param _addPoint 加算する値
		void AddStylishPoint(float _addPoint);

		/// @brief 攻撃情報をセットする
		/// @param _setAttackInfo 攻撃情報
		void SetAttackInfo(const AttackInformation& _setAttackInfo);

		/// @brief デバッグ用の無敵フラグをセットする
		/// @param _setBool 無敵にするか？
		static void SetIsDebugInvincible(bool _setBool);

		/// @brief 攻撃フラグを取得
		/// @return 攻撃フラグ
		bool GetIsAttackFlag() const;

		/// @brief デバッグ用無敵フラグを取得
		/// @return デバッグ用無敵か？
		static bool GetIsDebugInvincible();

		json Save() override;
		void Load(const json& _data) override;

		/// @brief 所有者のワールド座標を取得する
		/// @return 所有者のワールド座標
		const DXSimp::Vector3& GetOwnerWorldPos() const override;

		/// @brief 武器による攻撃ヒットさせたときに起こす処理
		void OnWeaponAttacking(const AttackInformation& _atkInfo) override;
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
		bool OnDamageBehavior(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos) override;
		void OnDeathBehavior() override;

		void ImGuiDebug() override;
		void ImGuiFindObj();
	};
}