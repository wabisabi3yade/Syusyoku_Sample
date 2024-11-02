#pragma once
#include "AnimationData.h"
#include "AnimationCurve.h"

class BoneList;

/// @brief �A�j���[�V�����m�[�h�Ɋ��N���X
class AnimationNode_Base : public HashiTaku::IImGuiUser, public HashiTaku::ISaveLoad
{
public:
	// �m�[�h�̎��
	enum class NodeType
	{
		Single,	// ������̃A�j���[�V����
		Blend,	// �u�����h����Ă���A�j���[�V����
	};

#ifdef EDIT
	/// @brief �񋓌^�𕶎���ɕϊ�
	static std::vector<std::string> edit_nodeTypeStrings;
#endif // EDIT
private:
	/// @brief �m�[�h��
	std::string nodeName;

	/// @brief �A�j���[�V�����J�[�u
	std::unique_ptr<AnimationCurve> pAnimationCurve;

	/// @brief �m�[�h�̎��
	NodeType nodeType;

	/// @brief ���݂̍Đ�����
	float curPlayingRatio;

	/// @brief ���݂̍Đ�����
	float lastPlayingRatio;

	/// @brief �m�[�h�̍Đ����x�{��(1.0�͓���)
	float playNodeSpeedTimes;

	/// @brief �A�j���[�V�����̎���
	float animationTime;

	/// @brief ���[�v�Đ����邩�H
	bool isLoop;

	/// @brief �A�j���[�V�����I�����Ă��邩�H
	bool isFinish;

	/// @brief �ړ����W�̃��[�g���[�V������K�p���邩(XZ���W)
	bool isRootMotionPosXZ;

	/// @brief �ړ����W�̃��[�g���[�V������K�p���邩(Y���W)
	bool isRootMotionPosY;

	/// @brief ��]�ʂ̃��[�g���[�V������K�p���邩
	bool isRootMotionRot;
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type);
	virtual ~AnimationNode_Base() {}
	/*AnimationNode_Base(const AnimationNode_Base& _other);
	AnimationNode_Base& operator=(const AnimationNode_Base& _other);*/

	// �Đ����̂Ƃ��ɕ\��
	virtual void ImGuiPlaying();

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V������
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// �m�[�h�����Z�b�g
	void SetNodeName(const std::string& _nodeName);

	// ���[�v���邩�Z�b�g
	void SetIsLoop(bool _isLoop);

	/// @brief �I���t���O���Z�b�g����
	void SetFinish();

	// �m�[�h���擾
	const std::string& GetNodeName() const;

	// �m�[�h�̎�ނ��擾
	NodeType GetNodeType() const;

	// ���݂̍Đ��������擾
	float GetCurPlayRatio() const;

	/// @brief 1�t���[���O�̍Đ��������擾
	float GetLastPlayRatio() const;

	// �Đ����Ԃ��擾
	float GetAnimationTime() const;

	/// @brief ���[�v�Đ��t���O���擾
	/// @return ���[�v�Đ��t���O
	bool GetIsLoop() const;

	// �A�j���[�V�����I���t���O���擾����
	bool GetIsFinish() const;

	/// @brief XZ�������[�g���[�V�����ňړ����邩�H
	/// @return �ړ����邩�H
	bool GetIsRootMotionXZ() const;

	/// @brief Y�������[�g���[�V�����ňړ����邩�H
	/// @return �ړ����邩�H
	bool GetIsRootMotionY() const;

	/// @brief ���[�g���[�V�����ŉ�]���邩�H
	/// @return ��]���邩�H
	bool GetIsRootMotionRot() const;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	/// @param _requestRatio �擾����w��̃A�j���[�V��������
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const {};

	/// @brief �Đ����x�{�������߂�
	/// @return �Đ����x�{��
	float GetPlaySpeedTimes() const;

	/// @brief �A�j���[�V�����J�[�u�̒l���擾����
	/// @param _ratio ����(0.0�`1.0)
	/// @return �A�j���[�V�����J�[�u�̒l(��{�I��0.0�`1.0)
	float GetCurveValue(float _ratio) const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	void Copy(const AnimationNode_Base& _other);

	// �m�[�h�̃p�����[�^�[�ݒ�
	void ImGuiSetParameter();
protected:
	/// @brief �X�V�������s��
	/// @param _boneList �{�[�����X�g
	virtual void Update(BoneList& _boneList) {};

	// �Đ����Ԃ��Z�b�g����
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;

};

