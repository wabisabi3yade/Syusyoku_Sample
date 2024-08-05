#include "pch.h"
#include "VariableFrameRate.h"

void VariableFrameRate::Init(u_int setFrameRate)
{
	// FPS�Œ�p�ϐ�
	long long frequency; // �v�����x
	// �v�����x���擾
	QueryPerformanceFrequency(&liWork);
	frequency = liWork.QuadPart; // �P�b������̉𑜓x������
	// �P�t���[��������̃J�E���g�l���v�Z
	microSecondsPerFrame = frequency / setFrameRate; // 60FPS�Ōv�Z

	previousFrameTime = GetNowTime();
}

bool VariableFrameRate::UpdateCheck()
{
	// �O�̃t���[������̕b���𑪂�
	auto currentTime = GetNowTime();
	auto count = currentTime - previousFrameTime;

	// 1�t���[�����ԂɒB������
	if (count < microSecondsPerFrame)
		return false;

	// �f���^�^�C��(ms)�ɂ���
	deltaTime = static_cast<double>(count * 0.0001f);

	// �X�V����
	previousFrameTime = currentTime;

	return true;
}

void VariableFrameRate::Draw()
{
	if (ImGui::TreeNode("FPS Counter"))
	{
		float fps = static_cast<float>(1.0f / (deltaTime * 0.001f));

		ImGui::Text("%lf FPS", fps);

		ImGui::TreePop();
	}
}

long long VariableFrameRate::GetNowTime()
{
	QueryPerformanceCounter(&liWork);
	return liWork.QuadPart;
}
