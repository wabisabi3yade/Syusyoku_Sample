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
	virtual DirectX::SimpleMath::Vector3 GetWorldPos() const = 0;

	/// @brief ターゲットする側を取得する
	/// @param _targeter ターゲットする側
	virtual void GetTargeter(IObjectTargeter& _targeter) = 0;

	/// @brief ターゲット側に死んだことを通知
	virtual void OnDeathNotifyTargeter() = 0;
};

/// @brief ターゲットする人のインターフェース
class IObjectTargeter
{

public:
	IObjectTargeter() {}
	virtual ~IObjectTargeter() {}

	/// @brief ターゲットオブジェクトを取得する
	/// @param _targetObject ターゲットオブジェクト
	virtual void GetTargetObject(ITargetAccepter& _targetObject) = 0;

	/// @brief ターゲットが死んだときの処理
	virtual void OnTargetDeath() = 0;
};

