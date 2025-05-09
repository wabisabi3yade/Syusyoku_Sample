#pragma once

namespace HashiTaku
{
	struct AcceptParryInfo
	{
		/// @brief パリィの強度による倍率
		float parryStrengthRate{ 1.0f };
	};

	/// @brief パリィを受け流した攻撃をした側のインターフェース(プレイヤーが敵の攻撃をパリィ　←　敵の方のインターフェース)
	class IParryAccepter
	{

	public:
		IParryAccepter() = default;
		virtual ~IParryAccepter() = default;

		/// @brief パリィされたときの処理
		/// @param _acceptInfo パリィからの情報
		virtual void OnAcceptParry(const AcceptParryInfo& _acceptInfo) = 0;
	};
}


