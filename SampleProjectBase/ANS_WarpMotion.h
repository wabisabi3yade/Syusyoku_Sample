//#pragma once
//#include "AnimationNotifyState.h"
//#include "AnimationCurve.h"
//
///// @brief ���[�g���[�V�������^�[�Q�b�g�̍��W�Ɉړ�������ʒm�C�x���g
//class ANS_WarpMotion : public AnimationNotifyState
//{
//	/// @brief �^�[�Q�b�g�̍��W�ֈړ����銄���̃J�[�u
//	AnimationCurve moveRatioCurve;
//
//	/// @brief �^�[�Q�b�g�̍��W
//	DirectX::SimpleMath::Vector3 targetPos;
//	
//	/// @brief 1�t���[���O�̍��W
//	DirectX::SimpleMath::Vector3 lastPos;
//
//	/// @brief 1�t���[���O�̊����l
//	float lastCurveValue;	
//public:
//	ANS_WarpMotion();
//	~ANS_WarpMotion() {}
//
//	/// @brief �^�[�Q�b�g�̍��W���Z�b�g
//	/// @param _targetPos �^�[�Q�b�g�̍��W
//	void SetTargetPosition(const DirectX::SimpleMath::Vector3& _targetPos);
//private:
//	void Begin() override;
//
//	/// @brief �C�x���g�Ԃ̍X�V����
//	void Tick() override;
//};
//
