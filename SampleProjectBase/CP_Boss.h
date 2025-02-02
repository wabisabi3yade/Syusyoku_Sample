#pragma once
#include "CP_Enemy.h"
#include "IAttacker_AcceptParry.h"
#include "CP_Weapon.h"
#include "BossActionController.h"
#include "BossAttackInformation.h"
#include "IUISlider.h"

namespace HashiTaku
{
	/// @brief ボスのコンポーネント
	class CP_Boss : public CP_Enemy, public IAttacker_AcceptParry
	{
		/// @brief 武器名
		std::string weaponObjName;

		/// @brief 体力バーオブジェクト名
		std::string hpBarObjName;

		/// @brief ブレイクバーオブジェクト名
		std::string breakBarObjName;

		/// @brief アクション管理
		std::unique_ptr<BossActionController> pActionController;

		/// @brief アニメーション
		CP_Animation* pAnimation;

		/// @brief 武器オブジェクト
		CP_Weapon* pWeapon;

		/// @brief カメラ移動
		CP_CameraMove* pCameraMove;

		/// @brief 体力バー
		IUISlider* pHpBar;

		/// @brief ブレイクゲージバー
		IUISlider* pBreakBar;

		/// @brief 最新の攻撃情報
		const BossAttackInformation* pRecentlyAttackInformation;

		/// @brief 攻撃判定
		const bool* pCanAttack;

		/// @brief ブレイク値の最大値
		float maxBreakValue;

		/// @brief 現在のブレイク値
		float curBreakValue;

		/// @brief ブレイクゲージを減らす速度
		float decadeBreakSpeed;

		/// @brief ブレイクしているか？
		bool isBreaking;
	public:
		CP_Boss();
		~CP_Boss() {}

		/// @brief 初期処理
		void Init() override;

		/// @brief 攻撃情報をセット
		/// @param _attackInfo 攻撃情報
		void SetAttackInfo(AttackInformation& _attackInfo);

		/// @brief 体力をセット
		/// @param _setHp 体力
		void SetCurrentHP(float _setHp) override;

		/// @brief ブレイク値をセット
		/// @param _set セット
		/// @param _canBreak 最大値を超えたときにブレイクするようにするか？
		void SetBreakValue(float _set, bool _canBreak = true);

		/// @brief ブレイクしているか取得
		/// @return ブレイクしているか？
		bool GetIsBreaking() const;

		/// @brief ブレイク値を取得
		/// @return ブレイク値
		float GetBreakValue() const;

		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;

		/// @brief 所有者のワールド座標を取得する
		/// @return 所有者のワールド座標
		const DXSimp::Vector3& GetAttackerWorldPos() const override;

		/// @brief 攻撃ヒットさせたときに起こす処理
		/// @param _atkInfo 攻撃情報
		/// @param _contactWorldPos 衝突地点（ワールド座標）
		void OnAttacking(const AttackInformation& _atkInfo,
			const DXSimp::Vector3& _contactWorldPos) override;

		/// @brief パリィされたときの処理
		/// @param _acceptInfo パリィからの情報
		void OnAcceptParry(const AcceptParryInfo& _acceptInfo) override;
	private:
		void Awake() override;
		void Start() override;
		void Update() override;
		void Draw() override;

		/// @brief 更新できるか取得
		/// @return 更新できるか？
		bool CanUpdate();

		/// @brief 攻撃できるか更新する
		void UpdateAttackCollision();

		/// @brief ブレイク関連更新処理
		void BreakUpdate();

		/// @brief 必要なオブジェクトを探す
		void FindRequaireObject();

		/// @brief ブレイク値を加算する
		/// @param _add 加算値
		/// @param _canBreak 最大値を超えたときにブレイクするようにするか？
		void AddBreakValue(float _add, bool _canBreak = true);

		/// @brief ブレイクした時の処理
		void OnBreak();

		/// @brief ブレイク終了時の処理
		void OnBreakEnd();

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _attackerPos 攻撃した側の座標
		/// @return ダメージを受けたか？
		bool OnDamageBehavior(AttackInformation& _attackInfo) override;

		/// @brief 死んだときの処理
		void OnDeathBehavior() override;

		void ImGuiDebug() override;
	};
}