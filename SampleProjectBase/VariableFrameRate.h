#pragma once



// �σt���[�����[�g�̕b�����v�Z����N���X
class VariableFrameRate
{
private:
	long long microSecondsPerFrame;	// 1�t���[�����Ƃ̃}�C�N���b
	double deltaTime;	// ��t�ims�j
	long long previousFrameTime;	// �O�̃t���[�����[�g���I�������Ƃ��̎���

	// FPS�Œ�p�ϐ�
	LARGE_INTEGER liWork; // �֐�����l���󂯎��p
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="setFrameRate">�Ή����������t���[�����[�g</param>
	VariableFrameRate() : microSecondsPerFrame(0), deltaTime(0), previousFrameTime(0), liWork() {}
	~VariableFrameRate() {}

	void Init(u_int setFrameRate);	// ����������

	// �Ή�����fps�ɂ��邽�߂Ɏ��̃��[�v�܂ő҂֐�
	bool UpdateCheck();

	// �t���[�����[�g�`��
	void Draw();

	// ��t���擾�i�b�ŕԂ��j
	float GetDeltaTime()const { return deltaTime * 0.001f ; }

private:

	/// @brief ���ݎ��Ԃ��擾����
	/// @return ���ݎ��ԁi0.1ns/�P�ʁj
	long long GetNowTime();
};

