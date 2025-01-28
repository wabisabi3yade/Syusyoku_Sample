#pragma once

namespace HashiTaku
{
	/// @brief 武器所持しているオブジェクトのインターフェース
	class IWeaponOwner
	{

	public:
		IWeaponOwner() = default;
		virtual ~IWeaponOwner() = default;

		/// @brief 所有者のワールド座標を取得する
		/// @return 所有者のワールド座標
		virtual const DXSimp::Vector3& GetOwnerWorldPos() const = 0;

		/// @brief 武器による攻撃ヒットさせたときに起こす処理
		virtual void OnWeaponAttacking() = 0;
	};

}

