//#pragma once
//#include "BossAttackState.h"
//
///// @brief �{�X�̃R���r�l�[�V�����U�����Z�b�g
//class BossCombAttack : public BossAttackState
//{
//	/// @brief �O�i�ރJ�[�u
//	AnimationCurve progressCurve;
//
//public:
//	BossCombAttack();
//	virtual ~BossCombAttack() {}
//
//	/// @brief �J�n����
//	void OnStartBehavior() override;
//
//	/// @brief �X�V����
//	void UpdateBehavior() override;
//
//	nlohmann::json Save() override;
//	void Load(const nlohmann::json& _data) override;
//private:
//	/// @brief �R���r�l�[�V�����U���̍U���ɍ��킹�čU�������X�V����
//	void UpdateReAttack();
//
//	/// @brief �U���񐔂��Z�b�g
//	/// @param _attackTime �U����
//	void SetAttackTimeCnt(u_int _attackTime);
//
//	void ImGuiDebug();
//};
//
