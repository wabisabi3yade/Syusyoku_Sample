#pragma once

struct aiNodeAnim;

class NodeAnim
{
	const aiNodeAnim* pAiNodeAnim;

	u_int boneIdx;	// �A�j���[�V�����œ������{�[����ID

public:
	NodeAnim();
	~NodeAnim();

	DirectX::SimpleMath::Vector3 GetScaling(float _playTime) const;
	DirectX::SimpleMath::Quaternion GetQuaternion(float _playTime) const;
	DirectX::SimpleMath::Vector3 GetPosition(float _playTime) const;

	// �Ή��{�[����ID���Z�b�g
	void SetBoneIdx(u_int _boneIdx);

	// �Ή��{�[����ID���擾
	u_int GetBodeIdx() const;

private:
	// �Đ����Ԃ���e�p�����[�^�̃L�[���擾����
	// �����F_playingTime �Đ�����
	u_int FindPreviousScaleKey(float _playingTime);
	u_int FindPreviousRotKey(float _playingTime);
	u_int FindPreviousPosKey(float _playingTime);
};

