#pragma once
#include "PlayerGroundState.h"
#include "PerlinShakeParameter.h"

namespace HashiTaku
{
	class CP_CameraMove;

	class PlayerGuardState : public PlayerGroundState
	{
		/// @brief パリィ時のエフェクト情報
		CreateVfxInfo parryEffectInfo;

		/// @brief パリィ時のカメラを揺らすパラメーター
		PerlinShakeParameter parryCamShakeParam;

		/// @brief パリィの効果音
		std::list<PlaySoundParameter> parrySoundParameters;

		/// @brief パリィエフェクトを生成する場所のオフセット(オブジェクトから)
		DXSimp::Vector3 createVfxOffset;

		/// @brief カメラ移動クラス
		CP_CameraMove* pCameraMove;

		/// @brief パリィできる状態の持続時間W
		u_int sustainParryFrame;

		/// @brief 経過時間
		u_int parryElapsedFrame;

		/// @brief パリィで増えるゲージ
		float parryAddGuardGage;

		/// @brief パリィすることができる正面の角度
		float canParryForwardAngle;

		/// @brief パリィ成功時に増やすスタイリッシュポイント
		float addStylishPointOnParry;

		/// @brief パリィ時パッド振動力
		float parryPadVibePower;

		/// @brief パリィ時パッド振動時間
		float parryPadVibeTime;

		/// @brief パリィできる状態
		bool canParry;

		/// @brief パリィを繰り出すフレームか？
		bool isPerformParryFrame;
	public:
		PlayerGuardState();
		~PlayerGuardState() {}

		/// @brief パリィできるか確認
		/// @return パリィできるか？
		bool GetCanParry(const DXSimp::Vector3& _enemyPos);

		/// @brief パリィ成功処理を行うようにする
		void SetPerfomParry();

		/// @brief セーブする
		/// @return セーブデータ
		json Save() override;

		/// @brief ロードする
		/// @param _data ロードするデータ 
		void Load(const json& _data) override;
	private:
		/// @brief 開始
		void OnStartBehavior() override;

		/// @brief 更新
		void UpdateBehavior() override;

		/// @brief 終了
		void OnEndBehavior() override;

		/// @brief アニメーション終了時の行動
		/// @param _fromAnimNodeName 遷移元のアニメーションノード名
		/// @param _toAnimNodeName 遷移先のアニメーションノード名
		void OnAnimationEnd(const std::string& _fromAnimNodeName,
			const std::string& _toAnimNodeName) override;

		/// @brief パリィ時の行動
		void OnParry();

		/// @brief パリィできる時間の更新
		void ParryTimeUpdate();

		/// @brief パリィを繰り出すかチェックする
		void CheckPerformParry();

		/// @brief ゲージを消費して強力な一撃
		void ReleaseAttack();

		/// @brief パリィエフェクトを生成
		void CreateParryVfx();

		/// @brief カメラシェイクを行う
		void CameraShakeOnParry();

		/// @brief コントローラーを振動
		void PadVibrationOnParry();

		/// @brief 効果音を鳴らす
		void PlayParrySE();

		void ImGuiDebug() override;
	private:
		// ガード時のパラメータ名
		static constexpr auto GUARD_PARAMNAME{ "isGuard" };
	};
}