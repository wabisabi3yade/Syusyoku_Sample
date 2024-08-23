#pragma once
#include "AnimationNode_Base.h"

/// @brief 2�ȏ�̃A�j���[�V�������u�����h����Ă���m�[�h
class BlendAnimationNode : public AnimationNode_Base
{
public:
	// �u�����h�Ɏg�p����f�[�^
	struct BlendData
	{
		const AnimationData* pAnimation{ nullptr }; // �A�j���[�V�����f�[�^
		float ratio{ 0.0f };	// �u�����h�̊���(0.0�`1.0)
	};

private:
	// �u�����h����Ƃ��̃y�A
	struct BlendPair
	{
		BlendData* prevData{ nullptr };	// �O
		BlendData* nextData{ nullptr };	// ��
	};

	/// @brief  �u�����h����A�j���[�V�����f�[�^
	std::list<BlendData> blendDatas;

	/// @brief ���݂̊���
	float curBlendRatio{ 0.0f };

	/// @brief �^�[�Q�b�g����
	float targetBlendRatio{ 0.0f };

	/// @brief �^�[�Q�b�g�������莞�̊���
	float changeBlendRatio{ 0.0f };

	/// @brief �^�[�Q�b�g�̊����ֈړ�����܂ł̎���
	float ratioSmoothTime{ 1.0f };

	/// @brief ���݂̊����ړ�����
	float curRatioSmoothTime{ 0.0f };

	/// @brief �^�[�Q�b�g�̊����ֈړ��C�[�W���O
	HashiTaku::EaseKind ratioMoveEase{ HashiTaku::EaseKind::OutCubic };
public:
	BlendAnimationNode(std::string _nodeName);

	~BlendAnimationNode() {}

	void ImGuiPlaying() override;

	/// @brief �X�V�������s��
	/// @param _playingRatio �Đ��̊���
	/// @param _boneList �{�[�����X�g
	void Update(float _playingRatio, BoneList& _boneList) override;

	/// @brief �A�j���[�V������ǉ�����
	/// @param _animData �A�j���[�V�����f�[�^
	void SetAnimationData(const std::string& _animName) override;

	/// @brief �ڕW�̊������Z�b�g����
	/// @param _ratio ����
	void SetTargetBlendRatio(float _ratio);

	/// @brief �A�j���[�V�����̊�����ς���B
	/// @param _ratio ����
	/// @param _animName �ύX����A�j���[�V����
	void SetAnimationRatio(float _ratio, const std::string& _animName);
private:
	/// @brief ���݂̃u�����h�l���ړ�����
	void MoveCurBlend();

	/// @brief �A�j���[�V�����̍X�V����
	/// @param _playingRatio �Đ�����
	/// @param _boneList �X�V����{�[�����X�g
	void AnimationUpdate(float _playingRatio, BoneList& _boneList);

	/// @brief �X�V�������s����̂�
	/// @return �X�V�ł���Ԃ�
	bool IsCanUpdate();

	/// @brief ���݂̊�������u�����h���ׂ��y�A��T��
	/// @return �u�����h���ׂ�2�̃u�����h�f�[�^
	BlendPair FindBlendPair();

	/// @brief �P��̃A�j���[�V�����X�V
	/// @param _boneList �{�[���z��
	/// @param _playingRatio �Đ�����
	/// @param _boneList �{�[���z��
	void SingleUpdateAnimation(BlendData& _animationData, float _playingRatio, BoneList& _boneList);

	/// @brief �u�����h�A�j���[�V�����X�V
	/// @param _blendPair �u�����h����2�̃f�[�^
	/// @param _playingRatio �Đ�����
	/// @param _boneList �{�[���z��
	void BlendUpdateAnimation(BlendPair& _blendPair, float _playingRatio, BoneList& _boneList);

	/// @brief �u�����h�f�[�^���A�j���[�V����������T��
	/// @param _animName �A�j���[�V������
	/// @return �Ή�����u�����h�f�[�^
	BlendData* FindBlendData(const std::string& _animName);

	/// @brief 0.0�`1.0�͈͓̔��ɂ���
	/// @param _ratio �Z�b�g����銄���̎Q��
	/// @param _setRatio �Z�b�g���銄��
	void SetRatio(float& _ratio, float _setRatio);

	/// @brief �u�����h�l�������Ƀ\�[�g����
	void SortBlendValue();

	/// @brief �f�[�^���Ƃ̃u�����h�l�̔�r�֐�
	/// @param _bd1 �f�[�^1
	/// @param _bd2 �f�[�^2
	/// @return  ����ւ���Ȃ�
	static bool CompareBlendValue(const BlendData& _bd1, const BlendData& _bd2);

	/// @brief �A�j���[�V�����̃g�����X�t�H�[���擾
	/// @param _transforms �i�[����{�[���g�����X�t�H�[���z��
	/// @param _boneNum �{�[���̐�
	/// @param _requestKeyNum �擾�������L�[��
	void GetAnimTransform(std::vector<BoneTransform>& _transforms, u_int _boneNum, u_int _requestKeyNum) const override;
};

