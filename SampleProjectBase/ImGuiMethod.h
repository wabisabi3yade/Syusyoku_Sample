#pragma once

struct ImFont;


namespace HashiTaku
{
	/// @brief InputText�Ŏg�p����o�b�t�@��
	constexpr u_int IM_INPUT_BUF(256);

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
		/// @return �ύX�������H
		static bool DragFloat3(DXSimp::Vector3& _vector3, const std::string& _name, float _speed = 1.0f);

		/// @brief ColorEdit4��Color�ɑΉ��������֐�
		/// @param _color Color
		/// @param _name �ϐ���
		static void ColorEdit4(DXSimp::Color& _color, const std::string& _name);


		/// @brief map�z��p�R���{�{�b�N�X�쐬
		/// @param _caption �L���v�V����
		/// @param _currentItem ���ݑI�𒆂̖��O
		/// @param _items �v�f�̑S���O
		/// @return �ύX�������H
		static bool ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<const std::string*>& _items);

		/// @brief enum�p�R���{�{�b�N�X�쐬
		/// @param _caption �L���v�V����
		/// @param _enumId enum�̐���
		/// @param _items �񋓌^�̖��O
		/// @return �ύX�������H
		static bool ComboBox(const std::string& _caption, u_int& _enumId, const std::vector<std::string>& _items);
		static bool ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<std::string>& _items);

		/// @brief �c���[�m�[�h
		/// @return �m�[�h���J����Ă��邩�H
		static bool TreeNode(const std::string& _caption);

		/// @brief �N���b�N����ƁA�e�L�X�g���͂ł���
		/// @param _caption �L���v�V����
		/// @param _text �ύX�������e�L�X�g
		/// @param _isEditing �ҏW�����ǂ���
		/// @return �m�[�h���J����Ă��邩�H
		static bool EditableText(const std::string& _caption, std::string& _text, bool& _isEditing);

		/// @brief Vector2���e�L�X�g
		/// @param _v �\������e�L�X�g
		static void Text(const DXSimp::Vector2& _v);

		/// @brief Vector3���e�L�X�g
		/// @param _v �\������e�L�X�g
		static void Text(const DXSimp::Vector3& _v);

		/// @brief Quaternion���e�L�X�g
		/// @param _q �\������e�L�X�g
		static void Text(const DXSimp::Quaternion& _q);

		/// @brief bool���e�L�X�g
		/// @param _b �\������e�L�X�g
		/// 

		/// @brief bool���e�L�X�g
		/// @param _text �\������e�L�X�g
		/// @param _b �\�����������p�����[�^
		static void Text(const std::string& _text, const bool _b);

		// �����_���ȃ��j�[�NID���Z�b�g
		//static void PushRandID();

		/// @brief �s�̋󔒂�}������(���T�C�Y)
		static void LineSpaceSmall();

		/// @brief �s�̋󔒂�}������(���T�C�Y)
		static void LineSpaceMid();

		/// @brief ���̓t�B�[���h�̃T�C�Y�𒲐�����
		static void PushItemSmallWidth();

		/// @brief �t�H���g�̑傫����ς���
		/// @param _size �t�H���g�̑傫��
		static void SetFontSize(FontSize _size);

		/// @brief �t�H���g�̑傫����߂�
		static void PopFontSize();
	private:

		/// @brief �t�H���g��ǂݍ���
		static void ImGuiLoadFonts();
	};
}