#pragma once
#include "AnimationController.h"
#include "AnimNodePlayer_Base.h"
#include "AnimTransitionChecker.h"
#include "ChangeAnimObserver.h"

/// @brief AnimationController���Đ�����N���X
class AnimControllPlayer : public HashiTaku::IImGuiUser
{
	/// @brief �A�j���[�V�����̍X�V�X�e�[�g
	enum class UpdateState
	{
		PlayNode,	// �m�[�h�Đ�
		Transition,	// �J�ڒ�
	};

	/// @brief ���݂̃��[�g���[�V�������W�ʒu
	DirectX::SimpleMath::Vector3 rootMotionPos;

	/// @brief �Đ�����A�j���[�V�����R���g���[���[
	const AnimationController* pAnimController;

	/// @brief �R���g���[�����̃p�����[�^�𕡐������I�u�W�F�N�g���Ƃ̃p�����[�^���X�g
	std::unique_ptr<AnimationParameters> pCopyAnimParameters;

	/// @brief ���݂̃m�[�h�Đ��@�\
	std::unique_ptr<AnimNodePlayer_Base> pCurNodePlayer;

	/// @brief �ЂƂO�̃m�[�h�Đ��@�\
	std::unique_ptr<AnimNodePlayer_Base> pPrevNodePlayer;

	/// @brief �J�ڏ����𖞂����Ă��邩�ǂ����`�F�b�N����
	std::unique_ptr<AnimTransitionChecker> pTransChecker;

	/// @brief �N���X�t�F�[�h
	std::unique_ptr<CrossFadeAnimation> pCrossFadeInterp;

	/// @brief �������
	std::unique_ptr<InertInterpAnimation> pInertInterp;

	/// @brief �A�j���[�V�����ύX�T�u�W�F�N�g
	std::unique_ptr<HashiTaku::ChangeAnimSubject> pChangeAnimSubject;

	/// @brief �������Ώۂ̃{�[��
	BoneList* pBoneList;

	/// @brief �Q�[���I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform* pObjectTransform;

	/// @brief �Đ����x
	float playSpeed;

	/// @brief �A�j���[�V�����X�V�̏��
	UpdateState updateState;

	/// @brief �J�ڒ��̎��
	HashiTaku::AnimInterpolateKind curTransitonKind;
public:
	AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform);
	~AnimControllPlayer() {}

	/// @brief �X�V����
	void Update();

	/// @brief �T�u�W�F�N�g�ɃI�u�U�[�o�[��o�^
	/// @param _observer �I�u�U�[�o�[
	void AddChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief �T�u�W�F�N�g�ɃI�u�U�[�o�[���폜
	/// @param _observer �I�u�U�[�o�[
	void RemoveChangeAnimObserver(HashiTaku::ChangeAnimObserver& _observer);

	/// @brief �R�s�[���ꂽ���̃A�j���[�V�����p�����[�^
	/// @return �A�j���[�V�����p�����[�^�̎Q��
	AnimationParameters& GetCopyAnimParameters();

	/// @brief ���ݍĐ����Ă���m�[�h�Đ����擾
	/// @return �m�[�h�Đ�
	AnimNodePlayer_Base& GetCurNodePlayer() const;

	/// @brief ���[�g�{�[���̃��[�J�����W���擾
	/// @param _outPos ����
	void GetCurrentRootPos(DirectX::SimpleMath::Vector3& _outPos) const;

private:
	/// @brief �Đ��O������
	void PlayInit();

	/// @brief ������Ԃ��Z�b�g
	void SetDefault();

	/// @brief �e��Ԃ̍X�V����
	void StateUpdate();

	/// @brief �m�[�h���Đ�����
	void NodePlayUpdate();

	/// @brief �A�j���[�V�����J�ڍX�V����
	void TransitionUpdate();

	/// @brief �N���X�t�F�[�h�̍X�V����
	void CrossFadeUpdate();

	/// @brief ������Ԃ̍X�V����
	void InertializationUpdate();

	/// @brief �A�j���[�V�����J�ڂł��邩�m�F
	void TransitionCheck();
	
	/// @brief �A�j���[�V�����J�ڂ���^�C�~���O����
	/// @param _changeArrow �J�ڂ�����
	void OnChangeAnimation(const AnimTransitionArrow& _changeArrow);

	/// @brief �m�[�h����؂�ւ���
	/// @param _pChangeNodeInfo �ύX����m�[�h
	void ChangeNodePlayer(const AnimationNode_Base& _changeNode);

	/// @brief �N���X�t�F�[�h�J�ڂ̊J�n����
	void OnCrossFadeBegin(const AnimTransitionArrow& _changeArrow);

	/// @brief �A�j���[�V�����ύX�̃T�u�W�F�N�g���X�V����
	void ChangeAnimSubjectUpdate();

	void ImGuiSetting();
};

