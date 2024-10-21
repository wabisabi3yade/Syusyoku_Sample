#pragma once
class CP_RigidBody;

/// @brief �Փ˂���Ƃ��ɂǂ̊֐���I�����邩
class CollisionTypeJudge
{
public:
	/// @brief �Փ˂̎��
	enum class ColType
	{
		Already,	// ���ɍ��t���[���ł͍s����
		Enter,	// �ՓˊJ�n
		Stay,	// �Փ˒�
		Exit,	// �ՓˏI��
	};

private:
	/// @brief �O��t���[���̏Փ˂����Q�[���I�u�W�F�N�g���X�g
	std::unordered_set<const CP_RigidBody*> prevColRigidBodyCp;

	/// @brief ����t���[���̏Փ˂����Q�[���I�u�W�F�N�g���X�g
	std::unordered_set<const CP_RigidBody*> curColRigidBodyCp;

public:
	CollisionTypeJudge();
	~CollisionTypeJudge();

	/// @brief �Փˏ������s����ނ𔻒f����
	/// @param _colRbCp �Փˑ����Rb�R���|�[�l���g
	/// @return �Փ˂̎��
	ColType JudgeColKind(const CP_RigidBody& _colRbCp);

	/// @brief �ՓˏI�������Q�[���I�u�W�F�N�g���擾����
	/// @param _outGameObjects ���ʔz��
	void GetExitObjectList(std::vector<const CP_RigidBody*>& _outRbComponents);

	/// @brief ���t���[���ׂ̈̏���������
	void SetupForNextFrame();
};

