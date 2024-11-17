#pragma once

class IObjectTargeter;

/// @brief プレイヤーのターゲットの標的になるインターフェース
class ITargetAccepter
{

public:
	ITargetAccepter() {}
	virtual ~ITargetAccepter() {}

	/// @brief そのオブジェクトのワールド座標を取得
	/// @return ワールド座標を取得
	virtual const DirectX::SimpleMath::Vector3& GetWorldPosByTargetObj() const = 0;

	/// @brief ターゲットする側を取得する
	/// @param _targeter ターゲットする側
	virtual void GetTargeter(IObjectTargeter& _targeter) = 0;

	/// @brief ターゲット側に死んだことを通知
	virtual void OnDeathNotifyToTargeter() = 0;
};

/// @brief ターゲットする人のインターフェース
class IObjectTargeter
{

public:
	IObjectTargeter() {}
	virtual ~IObjectTargeter() {}

	/// @brief ターゲットオブジェクトを取得する
	/// @param _targetObject ターゲットオブジェクト
	virtual void SetTargetObject(ITargetAccepter& _targetObject) = 0;

	/// @brief ターゲットオブジェクトが死んだときの更新処理
	/// @param _deathTargetObj 死んだターゲットオブジェクト
	virtual void UpdateDeathNotify(const ITargetAccepter& _deathTargetObj) = 0;
};

