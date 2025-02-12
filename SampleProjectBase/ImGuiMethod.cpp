#include "pch.h"
#include "ImGuiMethod.h"

namespace HashiTaku
{
	// テキストサイズ
	constexpr float FONT_SIZES[static_cast<u_int>(ImGuiMethod::FontSize::Num)] = { 16.0f, 50.0f, 20.0f };
	constexpr int UNIQUE_ID_MIN(-2000000000);	// ユニークIDの最小値
	constexpr int UNIQUE_ID_MAX(2000000000);	// ユニークIDの最大値
	constexpr float LINESPACE_HEIGHT_S(5.0f);	// 行スペースの高さ(小)
	constexpr float LINESPACE_HEIGHT_M(10.0f);	// 行スペースの高さ(中)
	constexpr float BOX_SIZE(100.0f);	// 入力フィールドのサイズ
	// static変数初期化
	ImFont* ImGuiMethod::pFonts[static_cast<u_int>(FontSize::Num)] = {};

	using namespace DXSimp;

	void ImGuiMethod::Initialize(HWND _hwnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
	{
#ifdef EDIT
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();	// ImPlotも

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImWchar const ranges[] = { 0x0020, 0xfffd, 0, };

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// ImGuiのスタイルを設定
		ImGui::StyleColorsClassic();

		// ImGuiのバックエンドを初期化
		ImGui_ImplWin32_Init(_hwnd);
		ImGui_ImplDX11_Init(_pDevice, _pDeviceContext);

		// フォントを読み込む
		ImGuiLoadFonts();
#endif // EDIT
	}

	void ImGuiMethod::NewFrame()
	{
#ifdef EDIT
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
#endif // EDIT
	}

	void ImGuiMethod::EndFrame()
	{
#ifdef EDIT
		PopFontSize();
#endif // EDIT
	}

	void ImGuiMethod::Draw()
	{
#ifdef EDIT
		// ImGui　描画
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif // EDIT
	}

	void ImGuiMethod::Terminal()
	{
#ifdef EDIT
		// デバッグログ終了処理
		ImGuiDebugLog::Terminal();

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();

		ImPlot::DestroyContext();	// ImPlotも
		ImGui::DestroyContext();
#endif // EDIT
	}

	bool ImGuiMethod::DragFloat3(Vector3& _vector3, const std::string& _name, float _speed)
	{
#ifdef EDIT
		return ImGui::DragFloat3(TO_UTF8(_name), &_vector3.x, _speed);
#endif // EDIT
		return false;
	}

	void ImGuiMethod::ColorEdit4(DXSimp::Color& _color, const std::string& _name)
	{
#ifdef EDIT
		static float f[4];
		f[0] = _color.x;
		f[1] = _color.y;
		f[2] = _color.z;
		f[3] = _color.w;
		ImGui::ColorEdit4(TO_UTF8(_name), f);
		_color = { f[0], f[1], f[2], f[3] };	// 色
#endif // EDIT
	}

	bool ImGuiMethod::ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<const std::string*>& _items)
	{
#ifdef EDIT
		bool changed = false;

		if (ImGui::BeginCombo(_caption.c_str(), _currentItem.c_str()))
		{
			for (int n = 0; n < static_cast<int>(_items.size()); n++) {
				bool is_selected = (_currentItem == *_items[n]);
				if (ImGui::Selectable((*_items[n]).c_str(), is_selected))
				{
					_currentItem = *_items[n];
					changed = true;
					break;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		return changed;
#endif // EDIT
		return false;
	}

	bool ImGuiMethod::ComboBox(const std::string& _caption, u_int& _enumId, const std::vector<std::string>& _items)
	{
#ifdef EDIT
		std::string currentName = _items[_enumId];

		bool changed = ComboBox(_caption, currentName, _items);
		if (changed)
		{
			for (u_int i_i = 0; i_i < static_cast<u_int>(_items.size()); i_i++)
			{
				if (currentName == _items[i_i])
				{
					_enumId = i_i;
					break;
				}
			}
		}
		return changed;
#endif // EDIT
		return false;
	}

	bool ImGuiMethod::ComboBox(const std::string& _caption, std::string& _currentItem, const std::vector<std::string>& _items)
	{
#ifdef EDIT
		bool changed = false;

		if (ImGui::BeginCombo(_caption.c_str(), _currentItem.c_str()))
		{
			for (int n = 0; n < static_cast<int>(_items.size()); n++) {
				bool is_selected = (_currentItem == _items[n]);
				if (ImGui::Selectable(_items[n].c_str(), is_selected))
				{
					_currentItem = _items[n];
					changed = true;
					break;
				}
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}

			ImGui::EndCombo();
		}

		return changed;
#endif // EDIT

		return false;
	}

	bool ImGuiMethod::TreeNode(const std::string& _caption)
	{
#ifdef EDIT
		return ImGui::TreeNodeEx(_caption.c_str(), ImGuiTreeNodeFlags_Framed);
#endif // EDIT
		return false;
	}

	bool ImGuiMethod::EditableText(const std::string& _caption, std::string& text, bool& _isEditing)
	{
#ifdef EDIT
		static char buffer[IM_INPUT_BUF];        // テキスト編集用のバッファ

		if (_isEditing)
		{
			if (ImGui::InputText(_caption.c_str(), buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				// Enterキーが押されたら編集を終了し、テキストを更新
				text = std::string(buffer);
				_isEditing = false;  // 編集モードを終了
			}
			if (ImGui::IsItemDeactivated())
			{
				// フォーカスが外れたら編集モードを終了
				_isEditing = false;
			}
		}
		else
		{
			// 編集モードでない場合はラベルとして表示
			ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
			ImGui::TextColored(color, text.c_str());
			if (ImGui::IsItemClicked())
			{
				// クリックされたら編集モードに切り替え
				_isEditing = true;
				// 現在のテキストをバッファにコピー
				strcpy_s(buffer, text.c_str());
			}
		}
#endif // EDIT
		return false;
	}

	void ImGuiMethod::Text(const DXSimp::Vector2& _v)
	{
#ifdef EDIT
		ImGui::Text(std::string(
			"x " + std::to_string(_v.x) +
			"\ny " + std::to_string(_v.y)
		).c_str());
#endif // EDIT
	}

	void ImGuiMethod::Text(const Vector3& _v)
	{
#ifdef EDIT
		ImGui::Text(std::string(
			"x " + std::to_string(_v.x) +
			"\ny " + std::to_string(_v.y) +
			"\nz " + std::to_string(_v.z)
		).c_str());
#endif // EDIT
	}

	void ImGuiMethod::Text(const DXSimp::Quaternion& _q)
	{
#ifdef EDIT
		ImGui::Text(std::string(
			"x " + std::to_string(_q.x) +
			"\ny " + std::to_string(_q.y) +
			"\nz " + std::to_string(_q.z) +
			"\nw " + std::to_string(_q.w)
		).c_str());
#endif // EDIT
	}

	void ImGuiMethod::Text(const std::string& _text, const float _f)
	{
		std::string text = _text + ":" + std::to_string(_f);
		ImGui::Text(text.c_str());
	}

	void ImGuiMethod::Text(const std::string& _text, const bool _b)
	{
		std::string statusStr = _b ? "true" : "false";
		std::string text = _text + ":" + statusStr;
		ImGui::Text(text.c_str());
	}

	void ImGuiMethod::LineSpaceSmall()
	{
		ImGui::Dummy(ImVec2(0.0f, LINESPACE_HEIGHT_S));
	}

	void ImGuiMethod::LineSpaceMid()
	{
		ImGui::Dummy(ImVec2(0.0f, LINESPACE_HEIGHT_M));
	}

	void ImGuiMethod::PushItemSmallWidth()
	{
		ImGui::PushItemWidth(BOX_SIZE);
	}

	void ImGuiMethod::SetFontSize(FontSize _size)
	{
#ifdef EDIT
		ImGui::PushFont(pFonts[static_cast<u_int>(_size)]);
#endif // EDIT
	}

	void ImGuiMethod::PopFontSize()
	{
#ifdef EDIT
		ImGui::PopFont();
#endif // EDIT
	}

	void ImGuiMethod::ImGuiLoadFonts()
	{
#ifdef EDIT
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		const char* fontPath = "assets/font/MSGOTHIC.TTF";
		const u_int fontNum = static_cast<u_int>(FontSize::Num);

		// フォント読込
		for (int loop = 0; loop < fontNum; loop++)
		{
			io.Fonts->AddFontFromFileTTF(
				fontPath,
				FONT_SIZES[loop],
				nullptr,
				io.Fonts->GetGlyphRangesJapanese()
			);

			pFonts[loop] = io.Fonts->Fonts[loop];
		}

		// フォントテクスチャをリビルド
		ImGui_ImplDX11_InvalidateDeviceObjects();
		ImGui_ImplDX11_CreateDeviceObjects();
#endif // EDIT
	}
}