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

	 /// <summary>
	 /// �`�揈��
	 /// </summary>
	 static void Draw();

	 /// <summary>
	 /// �I������
	 /// </summary>
	 static void End();

	 /// <summary>
	 /// DragFloat3��Vector3�ɑΉ��������֐�
	 /// </summary>
	 /// <param name="_vector3">�Ή�������Vector3</param>
	 /// <param name="_name">�ϐ���</param>
	 static void DragFloat3(DirectX::SimpleMath::Vector3& _vector3, const std::string& _name);


	 /// <summary>
	 /// ColorEdit4��Color�ɑΉ��������֐�
	 /// </summary>
	 /// <param name="_vector3">�Ή�������Color</param>
	 /// <param name="_name">�ϐ���</param>
	 static void ColorEdit4(DirectX::SimpleMath::Color& _color, const std::string& _name);
};

