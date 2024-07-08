#pragma once

// �σt���[�����[�g�̕b�����v�Z����N���X
class VariableFrameRate
{
private:
	// 1�t���[�����Ƃ̕b
	double secondsPerFrame;	

	// ��t(s)
	double deltaTime;	

	// ���ݎ���
	ULONGLONG currentTime;

	// �O�̃t���[�����[�g���I�������Ƃ��̎���
	ULONGLONG previousFrameTime;	

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="setFrameRate">�Ή����������t���[�����[�g</param>
	VariableFrameRate(uint64_t setFrameRate);
	~VariableFrameRate();

	// ����������
	void Init();	

	// �Ή�����fps�ɂ��邽�߂Ɏ��̃��[�v�܂ő҂֐�
	void Wait();	 

	// ��t���擾�i�b�ŕԂ��j
	float GetDeltaTime()const { return static_cast<float>(deltaTime); }
};

