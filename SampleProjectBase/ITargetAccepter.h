#pragma once

namespace HashiTaku
{
	class IObjectTargeter;

	/// @brief プレイヤーのターゲットの標的になるインターフェース
	class ITargetAccepter
	{

	public:
		ITargetAccepter() {}
		virtual ~ITargetAccepter() {}

		/// @brief そのオブジェクトのワールド座標を取得
		/// @return ワールド座標を取得
		virtual const DirectX::SimpleMath::Vector3& GetWorldPos() const = 0;

		/// @brief ターゲットする側を取得する
		/// @param _targeter ターゲットする側
		virtual void SetTargeter(IObjectTargeter& _targeter) = 0;

		/// @brief ターゲットする側を削除する
		/// @param _targeter 削除ターゲットする側
		virtual void RemoveTargeter(IObjectTargeter& _targeter) = 0;

		/// @brief ターゲット側に解除することを通知
		virtual void OnRemoveNotifyToTargeter() = 0;
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

		/// @brief 解除するときの更新処理
		/// @param _removeObj 死んだターゲットオブジェクト
		virtual void RemoveNotify(const ITargetAccepter& _removeObj) = 0;
	};
}