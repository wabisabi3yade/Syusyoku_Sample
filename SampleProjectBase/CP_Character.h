#pragma once
#include "Component.h"
#include "IDamageable.h"

namespace HashiTaku
{
	/// @brief プレイヤーや敵の基底コンポーネント
	class CP_Character : public Component, public IDamageable
	{
	private:
		/// @brief 死んだフラグ
		bool isDead;

	protected:
		/// @brief 現在の体力
		float currentHP;

		/// @brief 最大体力
		float maxHP;

		/// @brief 最大体力制限
		static constexpr float MAXLIMIT_HP = 9999.9f;

	public:
		CP_Character();
		virtual ~CP_Character() {}

		/// @brief 最大体力をセット
		/// @param _hitPoint セットする体力
		void SetMaxHP(float _hitPoint);

		/// @brief 体力を取得
		/// @return 現在の体力
		float GetCurrentHP() const;

		/// @brief ダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		void OnDamage(const AttackInformation& _attackInfo);

		/// @brief 死んだときの処理
		void OnDeath();

		/// @brief セーブする
		/// @param _data セーブシーンデータ
		nlohmann::json Save() override;

		/// @brief ロードする
		/// @param _data ロードするシーンデータ 
		void Load(const nlohmann::json& _data) override;
	protected:
		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		virtual void OnDamageBehavior(const AttackInformation& _attackInfo) = 0;

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		virtual void OnDeathBehavior() = 0;

		void ImGuiSetting() override;
	private:
		void Start() override;
	};
}


