#pragma once
#include "AnimationController.h"
#include "IAnimationObserver.h"

// �v���C���[�̃A�j���[�V�����R���g���[���[
class PlayerAnimController : public AnimationController, public HashiTaku::AnimationSubject
{
public:
	enum class AnimType
	{
		Move,
		Attack,
		Jump,
		MaxNum
	};

private:
	std::unordered_map<AnimType, std::string> animTypeNodeNames;

	/// @brief ���x����
	float speedRatio;

	AnimType nowState;

public:
	PlayerAnimController();
	~PlayerAnimController() {}

	/// @brief ������animation�Z�b�g
	/// @param _type �A�j���[�V�����̃^�C�v
	void SetStartAnimation(AnimType _type);

	/// @brief �ړ����x�������Z�b�g
	/// @param _speedRatio �ړ����x����
	void SetMoveSpeedRatio(float _speedRatio);

private:
	/// @brief ��������
	void Init();

	/// @brief �m�[�h�쐬
	void InitCreateNode();

	/// @brief �J�ږ����쐬
	void InitTransitionArrow();

	/// @brief �A�j����ނ̗񋓌^�ƃm�[�h����Ή��Â���
	/// @param _animType �A�j�����
	/// @param _nodeName �A�j���m�[�h��
	void LinkAnimTypeNodeName(AnimType _animType, const std::string& _nodeName);

	/// @brief �A�j���[�V�����I������
	void OnAnimationFinish() override;

	/// @brief �A�j���[�V�����J�ڂ���
	/// @param _animName �A�j���[�V������
	/// @param  _isInterp ��Ԃ��邩�H
	virtual void ChangeAnimation(const std::string& _animName, float _targetAnimRatio = 0.0f, bool _isInterp = true);


	/// @brief ���O����񋓌^��T��
	/// @param _nodeName �m�[�h��
	AnimType FindAnimType(const std::string& _nodeName);

	void ImGuiSetting() override;
};

