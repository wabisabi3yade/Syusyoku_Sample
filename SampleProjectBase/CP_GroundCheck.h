#pragma once
#include "Component.h"
#include "IGroundNotifyer.h"

namespace HashiTaku
{
	/// @brief 地面の判定確認するを行うクラス
	class CP_GroundCheck : public Component
	{
		/// @brief 通知先のインターフェース
		IGroundNotifyer* pNotifyObject;

	public:
		CP_GroundCheck();
		virtual ~CP_GroundCheck() {}

		/// @brief 接地通知先オブジェクトをセット
		/// @param _notifyChecker 接地通知先オブジェクト
		void SetGroundNotifyer(IGroundNotifyer& _notifyChecker);

		/// @brief 接地通知先オブジェクトを外す
		/// @param _removeChecker 接地通知先オブジェクト
		void RemoveGroundNoifyer(IGroundNotifyer& _removeChecker);

		/// @brief 衝突開始
		/// @param _otherColInfo 衝突情報
		void OnCollisionEnter(const CollisionInfo& _otherColInfo) override;

		/// @brief 衝突終了
		/// @param _otherColInfo 衝突情報
		void OnCollisionExit(const CollisionInfo& _otherColInfo) override;
	};
}