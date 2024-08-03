#pragma once

/// @brief Vector3�^�p�����[�^�̃A�j���[�V�����L�[
struct AnimKey_V3
{
	DirectX::SimpleMath::Vector3 parameter;
	float startKeyNum;	// �J�n�̃L�[��
};

/// @brief Quaternion�^�p�����[�^�̃A�j���[�V�����L�[
struct AnimKey_Q
{
	DirectX::SimpleMath::Quaternion parameter;
	float startKeyNum;	// �J�n�̃L�[��
};

/// @brief 1�{�[���ɑ΂���A�j���[�V�������
class AnimationChannel
{
	/// @brief �m�[�h��
	std::string channelName;

	/// @brief ���W
	std::vector<AnimKey_V3> positionKeys;

	/// @brief �X�P�[��
	std::vector<AnimKey_V3> scaleKeys;

	/// @brief �N�H�[�^�j�I��
	std::vector<AnimKey_Q> quaternionKeys;

	/// @brief �Ή����Ă���{�[����ID
	int boneIdx;
public:
	AnimationChannel() : boneIdx(-1) {}
	~AnimationChannel() {}

	/// @brief ���W�L�[��ǉ�
	/// @param _startTime �J�n�L�[��
	/// @param _position ���W
	void AddPosKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _position);

	/// @brief �X�P�[���L�[��ǉ�
	/// @param _startTime �J�n�L�[��
	/// @param _scale �X�P�[���l
	void AddScaleKey(float _startKeyNum, const DirectX::SimpleMath::Vector3& _scale);

	/// @brief �N�H�[�^�j�I���L�[��ǉ�
	/// @param _startTime �J�n�L�[��
	/// @param _quaternion �N�H�[�^�j�I��
	void AddQuatKey(float _startKeyNum, const DirectX::SimpleMath::Quaternion& _quaternion);

	// �O�̃L�[����T��
	u_int FindPrevPosKey(float _playingTime, float _timePerKey) const;
	u_int FindPrevScaleKey(float _playingTime, float _timePerKey) const;
	u_int FindPrevQuatKey(float _playingTime, float _timePerKey) const;

	// �Ή��{�[����ID���Z�b�g
	void SetBoneIdx(u_int _boneIdx);

	// ���O���Z�b�g
	void SetName(const std::string& _name);

	/// @brief �L�[��������W���擾
	/// @param _keyNum �L�[��
	/// @return ���W�L�[
	const AnimKey_V3& GetPosKey(u_int _keyNum) const;

	/// @brief �L�[������X�P�[�����擾
	/// @param _keyNum �L�[��
	/// @return �X�P�[���L�[
	const AnimKey_V3& GetScaleKey(u_int _keyNum) const;

	/// @brief �L�[������N�H�[�^�j�I�����擾
	/// @param _keyNum �L�[��
	/// @return �{�[���̃N�H�[�^�j�I��
	const AnimKey_Q& GetQuatKey(u_int _keyNum) const;

	///// @brief �Đ����Ԃ�����W���擾����
	///// @param _playingTime �Đ�����
	///// @return �{�[���̍��W
	//DirectX::SimpleMath::Vector3 GetPosition(float _playingTime) const;

	///// @brief �Đ����Ԃ���X�P�[�����擾����
	///// @param _playingTime �Đ�����
	///// @return �{�[���̃X�P�[��
	//DirectX::SimpleMath::Vector3 GetScaling(float _playingTime) const;

	///// @brief �Đ����Ԃ���N�H�[�^�j�I�����擾����
	///// @param _playingTime �Đ�����
	///// @return �{�[���̃N�H�[�^�j�I��
	//DirectX::SimpleMath::Quaternion GetQuaternion(float _playingTime) const;

	// �e�L�[�̐����擾����
	u_int GetPosKeyCnt() const;
	u_int GetScaleKeyCnt() const;
	u_int GetQuatKeyCnt() const;

	// �Ή��{�[����ID���擾
	u_int GetBodeIdx() const;

	// ���O���擾
	std::string GetName();
private:

	//// �Đ����Ԃ����Ԃ��l�������p�����[�^���擾����
	//// _prevKeyNum�@�O�̃L�[��
	//// _playingTime �Đ�����
	//DirectX::SimpleMath::Vector3 CalcInterpolatedPosition(u_int _prevKeyNum, float _playingTime) const;
	//DirectX::SimpleMath::Vector3 CalcInterpolatedScale(u_int _prevKeyNum, float _playingTime) const;
	//DirectX::SimpleMath::Quaternion CalcInterpolatedQuaternion(u_int _prevKeyNum, float _playingTime) const;
};

