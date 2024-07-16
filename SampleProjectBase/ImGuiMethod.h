#pragma once

struct ImFont;

// imgui�̏������܂Ƃ߂��N���X(static�N���X)
class ImGuiMethod
{
public:
	// �t�H���g�̑傫��
	enum class FontSize
	{
		Small,	// ��
		Mid,	// ��
		Big,	// ��
		Num
	};

private:
	/// @brief �t�H���g���X�g(�T�C�Y�Ⴂ)
	static ImFont* pFonts[static_cast<u_int>(FontSize::Num)];

	ImGuiMethod() {}
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

	 /// @brief	�t���[���I������
	 static void EndFrame();

	 /// <summary>
	 /// �`�揈��
	 /// </summary>
	 static void Draw();

	 /// <summary>
	 /// �I������
	 /// </summary>
	 static void Terminal();
	 
	 /// @brief DragFloat3��Vector3�ɑΉ��������֐�
	 /// @param _vector3 Vector3
	 /// @param _name �ϐ���
	 /// @param _speed ���x
	 static void DragFloat3(DirectX::SimpleMath::Vector3& _vector3, const std::string& _name, float _speed = 1.0f);
	 
	 /// @brief ColorEdit4��Color�ɑΉ��������֐�
	 /// @param _color Color
	 /// @param _name �ϐ���
	 static void ColorEdit4(DirectX::SimpleMath::Color& _color, const std::string& _name);

	 /// @brief �t�H���g�̑傫����ς���
	 /// @param _size �t�H���g�̑傫��
	 static void SetFontSize(FontSize _size);

	 /// @brief �t�H���g�̑傫����߂�
	 static void PopFontSize();
private:

	/// @brief �t�H���g��ǂݍ���
	static void ImGuiLoadFonts();
};

