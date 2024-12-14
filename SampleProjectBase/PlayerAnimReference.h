#pragma once

constexpr auto SPEEDRATIO_PARAMNAME{ "speed" };	// 速度
constexpr auto TARGET_PARAMNAME{ "targeting" };	// ターゲット中か
constexpr auto GROUND_PARAMNAME{ "isGround" };	// 接地しているか
constexpr auto MOVEAXIS_X_PARAMNAME{ "axisX" };	// X移動
constexpr auto MOVEAXIS_Y_PARAMNAME{ "axisY" };	// Y移動
constexpr auto DAMAGETRIGGER_PARAMNAME{ "damageTrigger" };	// 受けダメージトリガー
constexpr auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };	// 攻撃トリガー
constexpr auto REATTACK_PARAMNAME{ "isReAttack" };	// リアタック時bool
constexpr auto CANCEL_PARAMNAME{ "canCancel" };	// キャンセル可能
// 先行入力できるか
constexpr auto INPUT_PARAMNAME{ "canInput" };
// 攻撃できるか
constexpr auto CANATK_PARAMNAME{ "canAttack" };
// 移動できるか
constexpr auto CANMOVE_PARAMNAME{ "canMove" };

// アニメーション名
constexpr auto IDLE_ANIM_NAME{ "Idle" };	// 待機状態