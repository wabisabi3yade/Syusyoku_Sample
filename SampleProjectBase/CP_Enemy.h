#pragma once
#include "CP_Character.h"
#include "ITargetAccepter.h"

class CP_Enemy : public CP_Character, public ITargetAccepter
{
	/// @brief 敵の名称
	std::string enemyName;

	/// @brief 自身をターゲットにしている者
	IObjectTargeter* pTargeter;

public:
	CP_Enemy();
	virtual ~CP_Enemy() {}

	/// @brief 敵名を取得
	/// @return 敵名
	const std::string& GetEnemyName() const;

	/// @brief そのオブジェクトのワールド座標を取得
	/// @return ワールド座標を取得
	const DirectX::SimpleMath::Vector3& GetWorldPosByTargetObj() const override;

	/// @brief ターゲットする側を取得する
	/// @param _targeter ターゲットする側
	void GetTargeter(IObjectTargeter& _targeter);

	/// @brief ターゲット側に死んだことを通知
	void OnDeathNotifyToTargeter();
protected:
	void Awake() override;
	void OnDestroy() override;

	/// @brief 敵名をセット
	/// @param _enemyName 敵名
	void SetEnemyName(const std::string& _enemyName);

	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	void OnDamageBehavior(const HashiTaku::AttackInformation& _attackInfo) override;

	/// @brief 各クラスのダメージを受けたときの処理
	/// @param _attackInfo 攻撃情報
	void OnDeathBehavior() override;
};

