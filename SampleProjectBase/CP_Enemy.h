#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"

namespace HashiTaku
{
	class CP_Enemy : public CP_Character, public ITargetAccepter
	{
		/// @brief 敵の名称
		std::string enemyName;

		/// @brief 自身をターゲットにしている者
		std::list<IObjectTargeter*> pTargeters;

	public:
		CP_Enemy();
		virtual ~CP_Enemy() {}

		/// @brief 敵名を取得
		/// @return 敵名
		const std::string& GetEnemyName() const;

		/// @brief そのオブジェクトのワールド座標を取得
		/// @return ワールド座標を取得
		const DXSimp::Vector3& GetWorldPos() const override;

		/// @brief ターゲットする側を取得する
		/// @param _targeter ターゲットする側
		void SetTargeter(IObjectTargeter& _targeter);

		/// @brief ターゲットする側を削除する
		/// @param _targeter 削除ターゲットする側
		void RemoveTargeter(IObjectTargeter& _targeter) override;

		/// @brief ターゲット側に死んだことを通知
		void OnRemoveNotifyToTargeter();
	protected:
		void Start() override;
		void OnDestroy() override;

		/// @brief 敵名をセット
		/// @param _enemyName 敵名
		void SetEnemyName(const std::string& _enemyName);

		/// @brief 各クラスのダメージを受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _attackerPos 攻撃した側の座標
		/// @return ダメージを受けたか？
		bool OnDamageBehavior(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _attackerPos) override;

		/// @brief ダメージ受けたときの処理
		/// @param _attackInfo 攻撃情報
		/// @param _contactPos 衝突地点
		void OnTakeDamage(const AttackInformation& _attackInfo,
			const DXSimp::Vector3& _contactPos) override;

		/// @brief 死んだときの処理
		void OnDeathBehavior() override {};
	};
}