#pragma once
#include "AnimationNotifyEvent.h"
#include "ITransformSetter.h"

namespace HashiTaku
{
	/// @brief アニメーションのタイミングでエフェクトを出す通知イベント
	class ANE_CreateVfx : public AnimationNotifyEvent, public ITransformSetter
	{
		/// @brief エフェクト情報
		CreateVfxInfo vfxInfo;

		/// @brief トランスフォームからのエフェクトを出すオフセット
		DXSimp::Vector3 createOffsetPos;

		/// @brief トランスフォーム情報
		const Transform* pTransform;

		/// @brief エフェクトのハンドル
		Effekseer::Handle vfxHandle;

		/// @brief 通知イベントが切り替わったときに直ぐにエフェクトを消すか？
		bool isExitDestroy;
	public:
		ANE_CreateVfx();
		~ANE_CreateVfx() {}
		
		/// @brief クローン関数
		/// @return 通知クラス
		std::unique_ptr<AnimationNotify_Base> Clone() override;

		/// @brief トランスフォームのセッター
		/// @param _transform トランスフォーム
		void SetTransform(const Transform& _transform) override;

		json Save() override;
		void Load(const json& _data) override;
	private:
		/// @brief イベント関数
		void OnEvent() override;

		/// @brief 切り替え前処理
		void OnTerminal() override;

		void ImGuiDebug() override;
	};
}


