#pragma once
#include "AnimationNode_Base.h"

/// @brief �A�j���[�V������񂪂ЂƂ����̃A�j���[�V�����m�[�h
class SingleAnimationNode : public AnimationNode_Base
{
	/// @brief �A�j���[�V�����f�[�^
	AnimationData* pAnimationData;
public:
	SingleAnimationNode(std::string _nodeName) : AnimationNode_Base(_nodeName, NodeType::Single), pAnimationData(nullptr) {}

	~SingleAnimationNode() {}

	void ImGuiPlaying() override;

	void Update(float _playingTime, BoneList& _boneList) override;

	/// @brief �A�j���[�V����
	/// @param _animData �A�j���[�V�����f�[�^
	void SetAnimationData(const std::string& _animName) override;

	// �A�j���[�V�����f�[�^���擾����
	const AnimationData& GetAnimationData() const;

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _transforms �i�[����{�[���g�����X�t�H�[���z��
	/// @param _boneNum �{�[���̐�
	/// @param _requestKeyNum �擾�������L�[��
	void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const override;
};