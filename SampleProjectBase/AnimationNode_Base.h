#pragma once
#include "AnimationData.h"

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
public:
	AnimationNode_Base(std::string _nodeName, NodeType _type);
	virtual ~AnimationNode_Base() {}
	AnimationNode_Base(const AnimationNode_Base& _other);
	AnimationNode_Base& operator=(const AnimationNode_Base& _other);

	// �Đ����̂Ƃ��ɕ\��
	virtual void ImGuiPlaying();

	/// @brief �A�j���[�V�����J�n���ɍs������
	virtual void Begin();

	/// @brief �X�V����
	/// @param _boneList �{�[�����X�g
	virtual void UpdateCall(BoneList& _boneList);

	/// @brief ���݂̍Đ�������i�߂�
	/// @param _animationSpeed �Đ����x
	void ProgressPlayRatio(float _animationSpeed);

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V������
	virtual void SetAnimationData(const std::string& _animName) = 0;

	// ���݂̍Đ��������Z�b�g
	void SetCurPlayRatio(float _playingRatio);

	// �m�[�h�����Z�b�g
	void SetNodeName(const std::string& _nodeName);

	// ���[�v���邩�Z�b�g
	void SetIsLoop(bool _isLoop);

	/// @brief �I���t���O���Z�b�g����
	void SetFinish();

	// �m�[�h���擾
	std::string GetNodeName() const;

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

	/// @brief ���݂̃A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	virtual void GetCurAnimTransform(BoneTransform& _outTransform, u_int _boneId) const = 0;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	/// @param _requestRatio �擾����w��̃A�j���[�V��������
	virtual void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const = 0;

	/// @brief �Đ����x�{�������߂�
	/// @return �Đ����x�{��
	float GetPlaySpeedTimes() const;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:

	/// @brief ���[�v�ł��邩�`�F�b�N
	/// @return ���[�v�ł��邩�H
	bool IsCanLoop();

	void Copy(const AnimationNode_Base& _other);

	// �m�[�h�̃p�����[�^�[�ݒ�
	void ImGuiSetParameter();
protected:
	/// @brief �X�V�������s��
	/// @param _boneList �{�[�����X�g
	virtual void Update(BoneList& _boneList) = 0;

	// �Đ����Ԃ��Z�b�g����
	void SetAnimationTime(float _time);

	void ImGuiSetting() override;
};

