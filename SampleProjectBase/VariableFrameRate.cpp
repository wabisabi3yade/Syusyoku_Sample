#include "pch.h"
#include "VariableFrameRate.h"

void VariableFrameRate::Init(u_int setFrameRate)
{
	FPS = setFrameRate;

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

	// 1�t���[�����ԂɒB���Ă��Ȃ��Ȃ�
	if (count < microSecondsPerFrame) return false;

	// �f���^�^�C��(ms)�ɂ���
	deltaTime_ms = static_cast<double>(count * 0.0001f);

	// �X�V����
	previousFrameTime = currentTime;

	return true;
}

void VariableFrameRate::ImGuiSetting()
{
#ifdef EDIT
	if (ImGuiMethod::TreeNode("FPS Counter"))
	{
		float fps = static_cast<float>(1.0f / (deltaTime_ms * 0.001f));

		ImGui::Text("Avarage:%lf ms", avarageProcessTime_ms);
		ImGui::Text("%lf FPS", fps);

		ImGui::TreePop();
	}
#endif
}

void VariableFrameRate::ProcessEnd()
{
#ifdef EDIT
	auto processTime = GetNowTime() - previousFrameTime;
	// 0.1ns����ms�ɕϊ�
	float processTime_ms = processTime * 0.0001f;

	processTimes.push_back(processTime_ms);

	if (static_cast<u_int>(processTimes.size()) < FPS) return;

	// 1�b���Ƃ̕��Ϗ������Ԃ����߂�
	float sumProcessTime = 0.0f;
	for (auto& p : processTimes)
		sumProcessTime += p;

	avarageProcessTime_ms = sumProcessTime / FPS;

	processTimes.clear();

#endif // EDIT
}

long long VariableFrameRate::GetNowTime()
{
	QueryPerformanceCounter(&liWork);
	return liWork.QuadPart;
}
