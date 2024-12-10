#pragma once
#include "AnimNodePlayer_Base.h"

/// @brief ���C���[�m�[�h�̍Đ��N���X
class LayerdNodePlayer : public AnimNodePlayer_Base
{

	
public:
	LayerdNodePlayer(const AnimationNode_Base& _playNode, BoneList& _boneList, Transform& _transform);
	~LayerdNodePlayer() {}

	/// @brief �A�j���[�V�����̍X�V����
	/// @param _outTransforms �A�j���[�V�����̎p�� 
	void Update(std::vector<BoneTransform>& _outTransforms) override;

	/// @brief ���[�g���[�V�����̍��W�ړ����x���v�Z����
	void CalcRootMotionPosSpeed() override;

private:
	void ImGuiDebug() override;
};

