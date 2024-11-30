#pragma once
#include "PlayerActState_Base.h"

class PlayerGuardState : public PlayerActState_Base
{
	/// @brief パリィできる状態の持続時間
	u_int sustainParryFrame;

	/// @brief 経過時間
	u_int parryElapsedFrame;

	/// @brief パリィで増えるゲージ
	float parryAddGuardGage;

	/// @brief ガードすることができる正面の角度
	float canParryForwardAngle;

	/// @brief パリィできる状態
	bool canParry;
public:
	PlayerGuardState();
	~PlayerGuardState() {}

	/// @brief パリィできるか確認
	/// @return パリィできるか？
	bool GetCanParry(const DirectX::SimpleMath::Vector3& _enemyPos);

	/// @brief パリィ時の行動
	void OnParry();

	/// @brief セーブする
	/// @return セーブデータ
	nlohmann::json Save() override;

	/// @brief ロードする
	/// @param _data ロードするデータ 
	void Load(const nlohmann::json& _data) override;
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

	/// @brief パリィできる時間の更新
	void ParryTimeUpdate();

	/// @brief ゲージを消費して強力な一撃
	void ReleaseAttack();

	/// @brief パリィ時の行動
	void GuardParry();

	void ImGuiDebug() override;
private:
	// ガード時のパラメータ名
	static constexpr auto GUARD_PARAMNAME{ "isGuard" };
};

