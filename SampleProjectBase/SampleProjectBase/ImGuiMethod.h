#pragma once
#include "ImGuiDebugLog.h"

// imgui�̏������܂Ƃ߂��N���X(static�N���X)
class ImGuiMethod
{
	ImGuiMethod();
	~ImGuiMethod() {};
 public:
	 /// <summary>
	 /// ImGui�̏���������
	 /// </summary>
	 /// <returns>�������̐������</returns>
	 static void Initialize(HWND _hwnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext);

	 /// <summary>
	 /// �t���[���X�V����
	 /// </summary>
	 static void NewFrame();

	 static void Draw();

	 /// <summary>
	 /// �I������
	 /// </summary>
	 static void End();
};

