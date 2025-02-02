#pragma once
#include "Component.h"
#include "IDamageable.h"
#include "HitStopper_Base.h"

namespace HashiTaku
{
	class CP_CameraMove;

	/// @brief プレイヤーや敵の基底コンポーネント
	class CP_Character : public Component, public IDamageable, public HitStopper_Base
	{
	private:
		/// @brief 死んだフラグ
		bool isDead;

		/// @brief ヒットストップ中か？
		bool isHitStopping;

		/// @brief 無敵フラグ
		bool isInvicible;
	protected:
		/// @brief 現在の体力
		float currentHP;

		/// @brief HSが入る前のオブジェクトの経過時間
		float hsBeforeDeltaTime;

		/// @brief 最大体力
		float maxHP;

		/// @brief 最大体力制限
		static constexpr float MAXLIMIT_HP = 9999.9f;
	public:
		CP_Character();
		virtual ~CP_Character() {}

		/// @brief オブジェクト削除時の行動
		void OnDestroy() override;

		/// @brief 最大体力をセット
		/// @param _hitPoint セットする体力
		virtual void SetMaxHP(float _hitPoint);

		/// @brief 無敵フラグをセット
		/// @param _isInvicible 無敵フラグ
		void SetIsInvicible(bool _isInvicible);

		/// @brief 体力を取得
		/// @return 現在の体力
		float GetCurrentHP() const;

		/// @brief 無敵フラグを取得
		/// @return 無敵か？
		bool GetIsInvicible() const;

		/// @brief 死んでいるか取得
		/// @return 死んでいるか？
		bool GetDead() const;

		/// @brief ダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		void OnDamage(AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief 死んだときの処理
		void OnDeath();

		/// @brief ヒットストップ開始した時の処理
		void OnHitStopBegin() override;

		/// @brief ヒットストップ終了した時の処理
		void OnHitStopEnd() override;

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const json& _data) override;
	protected:
		/// @brief ヒットストップ中華取得
		/// @return ヒットストップしてるか？
		bool GetIsHitStopping() const;

		void Start() override;

		/// @brief ヒットストップを開始する
		/// @param _hitStopFlame ヒットストップのフレーム数
		void BeginHitStop(u_int _hitStopFlame);

		/// @brief 体力をセット
		/// @param _setHp 体力
		virtual void SetCurrentHP(float _setHp);

		/// @brief HPを減らす
		/// @param _damageVal ダメージ値
		void DecadeHp(float _damageVal);

		/// @brief エフェクトを出す
		/// @param _attackInfo 攻撃情報
		/// @param 当たり判定の衝突地点
		void CreateVfx(const CreateVfxInfo& _vfxInfo,
			const DXSimp::Vector3& _createPos);

		/// @brief SEを再生
		/// @param _attackInfo 攻撃情報
		/// @param 当たり判定の衝突地点
		void CreateSoundFX(const PlaySoundParameter& _soundParam,
			const DXSimp::Vector3& _soundPos);

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _attackerPos 攻撃した側の座標
		/// @return ダメージを受けたか？
		virtual bool OnDamageBehavior(AttackInformation& _attackInfo) = 0;

		/// @brief ダメージ受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _contactPos 衝突地点
		virtual void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos);

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		virtual void OnDeathBehavior() = 0;

		void ImGuiDebug() override;
	};
}