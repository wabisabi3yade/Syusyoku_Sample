#pragma once

// imgui�̏������܂Ƃ߂��N���X(static�N���X)
class ImGuiMethod
{
	ImGuiMethod(){};
	~ImGuiMethod() {};

	//static ID3D11DeviceContext* pDeviceContext;	// �f�o�C�X�R���e�L�X�g
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

	 /// <summary>
	 /// �I������
	 /// </summary>
	 static void End();
};

