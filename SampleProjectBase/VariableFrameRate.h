#pragma once

// �σt���[�����[�g�̕b�����v�Z����N���X
class VariableFrameRate : public HashiTaku::IImGuiUser
{
private:
	long long microSecondsPerFrame;	// 1�t���[�����Ƃ̃}�C�N���b
	double deltaTime_ms;	// ��t�ims�j
	long long previousFrameTime;	// �O�̃t���[�����[�g���I�������Ƃ��̎���

	float avarageProcessTime_ms;	// 1�b���Ƃ̕��Ϗ�������
	std::vector<float> processTimes;	// �Z�b���Ƃ̕��ς��Ƃ邽�߂̕ۑ����X�g

	u_int FPS;	// fps

	// FPS�Œ�p�ϐ�
	LARGE_INTEGER liWork; // �֐�����l���󂯎��p
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="setFrameRate">�Ή����������t���[�����[�g</param>
	VariableFrameRate() : microSecondsPerFrame(0), deltaTime_ms(0), previousFrameTime(0), avarageProcessTime_ms(0.0f), FPS(0), liWork() {}

	~VariableFrameRate() {}

	void Init(u_int setFrameRate);	// ����������

	// �Ή�����fps�ɂ��邽�߂Ɏ��̃��[�v�܂ő҂֐�
	bool UpdateCheck();

	/// @brief �����I�����ɌĂяo��
	void ProcessEnd();

	// ��t���擾�i�b�ŕԂ��j
	float GetDeltaTime()const { return static_cast<float>(deltaTime_ms * 0.001f); }

private:
	/// @brief ���ݎ��Ԃ��擾����
	/// @return ���ݎ��ԁi0.1ns/�P�ʁj
	long long GetNowTime();

	// �t���[�����[�g�`��
	void ImGuiSetting() override;
};

