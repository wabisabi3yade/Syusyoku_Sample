#pragma once

namespace HashiTaku
{
	constexpr auto SPEEDRATIO_PARAMNAME{ "speed" };	// ���x
	constexpr auto TARGET_PARAMNAME{ "targeting" };	// �^�[�Q�b�g����
	constexpr auto GROUND_PARAMNAME{ "isGround" };	// �ڒn���Ă��邩
	constexpr auto MOVEAXIS_X_PARAMNAME{ "axisX" };	// X�ړ�
	constexpr auto MOVEAXIS_Y_PARAMNAME{ "axisY" };	// Y�ړ�
	constexpr auto DAMAGETRIGGER_PARAMNAME{ "damageTrigger" };	// �󂯃_���[�W�g���K�[
	constexpr auto ATTACKTRIGGER_PARAMNAME{ "attackTrigger" };	// �U���g���K�[
	constexpr auto REATTACK_PARAMNAME{ "isReAttack" };	// ���A�^�b�N��bool
	constexpr auto CHARGE_PARAMNAME{ "isCharging" };	// �`���[�W���Ȃ̂�
	constexpr auto CHARGE_STEP_PARAMNAME{ "chargeSteps" };	// �`���[�W���̃X�e�b�v
	constexpr auto INPUT_PARAMNAME{ "canInput" };	// ��s���͂ł��邩
	constexpr auto CANCEL_PARAMNAME{ "canCancel" };	// �L�����Z���\
	constexpr auto CANATK_PARAMNAME{ "canAttack" };	// �U���ł��邩
	constexpr auto CANMOVE_PARAMNAME{ "canMove" };	// �ړ��ł��邩

	// �A�j���[�V������
	constexpr auto IDLE_ANIM_NAME{ "Idle" };	// �ҋ@���
}