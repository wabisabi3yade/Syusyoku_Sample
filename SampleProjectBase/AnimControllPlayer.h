#pragma once
#include "AnimationController.h"
#include "AnimNodePlayer_Base.h"

/// @brief AnimationController���Đ�����N���X
class AnimControllPlayer : public HashiTaku::IImGuiUser
{
	enum class PlayState
	{
		PlayNode,
		Transition,
	};

	/// @brief �Đ�����A�j���[�V�����R���g���[���[
	const AnimationController* pAnimController;

	/// @brief ���ݍĐ����Ă���m�[�h���
	const AnimationController::AnimNodeInfo* pNodeInfo;

	/// @brief �R���g���[�����̃p�����[�^�𕡐������I�u�W�F�N�g���Ƃ̃p�����[�^���X�g
	std::unique_ptr<AnimationParameters> pCopyAnimParameters;

	/// @brief ���݂̃m�[�h�Đ��@�\
	std::unique_ptr<AnimNodePlayer_Base> pCurNodePlayer;

	/// @brief �ЂƂO�̃m�[�h�Đ��@�\
	std::unique_ptr<AnimNodePlayer_Base> pPrevNodePlayer;

	/// @brief �������Ώۂ̃{�[��
	BoneList* pBoneList;

	/// @brief �Q�[���I�u�W�F�N�g�̃g�����X�t�H�[��
	Transform* pObjectTransform;

	/// @brief �Đ����x
	float playSpeed;

	/// @brief �v���C���[�̏��
	PlayState playState;
public:
	AnimControllPlayer(const AnimationController& _animController, BoneList& _boneList, Transform& _transform);
	~AnimControllPlayer() {}

	/// @brief �X�V����
	void Update();

	/// @brief �m�[�h����؂�ւ���
	/// @param _pChangeNodeInfo �ύX����m�[�h���
	void ChangeNodeInfo(const AnimationController::AnimNodeInfo& _changeNodeInfo);

	/// @brief �R�s�[���ꂽ���̃A�j���[�V�����p�����[�^
	/// @return �A�j���[�V�����p�����[�^�̎Q��
	AnimationParameters& GetCopyAnimParameters();

private:
	/// @brief �Đ��O������
	void PlayInit();

	/// @brief �m�[�h���Đ�����
	void NodePlay();

	void ImGuiSetting();
};

