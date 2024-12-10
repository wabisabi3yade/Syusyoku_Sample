#pragma once
#include "AnimationNode_Base.h"

/// @brief ���ʂ��Ƃɕ����ăA�j���[�V��������m�[�h
class LayerdAnimationNode : public AnimationNode_Base
{
	// ����㔼�g�A�����g�̍Đ�����������̂�2�����Ή����܂��A�K�v�ɂȂ�����g��

	/// @brief ��b�ƂȂ�A�j���[�V����
	AnimationData* pBaseAnimation;

	/// @brief �u�����h����A�j���[�V����
	AnimationData* pBlendAnimation;

	/// @brief �ǂ�����u�����h�A�j���[�V�������Đ����邩�w�肷��{�[��ID
	int beginBlendBoneId;
public:
	/// @brief �R���X�g���N�^
	/// @param _nodeName �m�[�h��
	LayerdAnimationNode(const std::string& _nodeName);
	~LayerdAnimationNode() {}

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _outTransform �i�[����{�[���g�����X�t�H�[��
	/// @param _boneId �{�[����ID
	/// @param _requestRatio �擾����w��̃A�j���[�V��������
	void GetAnimTransform(BoneTransform& _outTransform, u_int _boneId, float _requestRatio) const override;

	nlohmann::json Save() override;
	void Load(const nlohmann::json& _data) override;
private:
	/// @brief �@�\�ł��Ă��邩�m�F����
	/// @return �@�\�ł��Ă��邩�H
	bool CanWarking() const;

	void ImGuiDebug() override;

	// �u�����h�̃{�[ID���擾����
	void ImGuiGetBlendBoneId();
};

