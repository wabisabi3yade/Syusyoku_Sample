#pragma once

// �σt���[�����[�g�̕b�����v�Z����N���X
class VariableFrameRate
{
private:
	uint64_t microSecondsPerFrame;	// 1�t���[�����Ƃ̃}�C�N���b
	uint64_t deltaTime;	// ��t�i�~���b�j
	uint64_t currentTime;	// ���ݎ���
	uint64_t previousFrameTime;	// �O�̃t���[�����[�g���I�������Ƃ��̎���

	LARGE_INTEGER frequency;
	LARGE_INTEGER startTime, endTime;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="setFrameRate">�Ή����������t���[�����[�g</param>
	VariableFrameRate(uint64_t setFrameRate);
	~VariableFrameRate();

	void Init();	// ����������

	void CaluculateDelta();	// ��t���v�Z����

	// �Ή�����fps�ɂ��邽�߂Ɏ��̃��[�v�܂ő҂֐�
	void Wait();	 

	// ��t���擾�i�b�ŕԂ��j
	float GetDeltaTime()const { return deltaTime / 1000.0f; }
};

