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
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	/// @param _requestRatio �擾����w��̃A�j���[�V��������
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;
};