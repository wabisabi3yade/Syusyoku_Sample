#include "pch.h"
#include "ImGuiMethod.h"

// テキストサイズ
constexpr float FONT_SIZES[static_cast<u_int>(ImGuiMethod::FontSize::Num)] = { 16.0f, 50.0f, 20.0f };

// static変数初期化
ImFont* ImGuiMethod::pFonts[static_cast<u_int>(FontSize::Num)] = {};

using namespace DirectX::SimpleMath;

void ImGuiMethod::Initialize(HWND _hwnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
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
}

void ImGuiMethod::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiMethod::EndFrame()
{
	PopFontSize();
}

void ImGuiMethod::Draw()
{
	ImGuiDebugLog::DisplayMessage();

	// ImGui　描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiMethod::Terminal()
{
	// デバッグログ終了処理
	ImGuiDebugLog::Terminal();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ImGuiMethod::DragFloat3(Vector3& _vector3, const std::string& _name, float _speed)
{
	return ImGui::DragFloat3(TO_UTF8(_name), &_vector3.x, _speed);
}

void ImGuiMethod::ColorEdit4(DirectX::SimpleMath::Color& _color, const std::string& _name)
{
	static float f[4];
	f[0] = _color.x;
	f[1] = _color.y;
	f[2] = _color.z;
	f[3] = _color.w;
	ImGui::ColorEdit4(TO_UTF8(_name), f);
	_color = { f[0], f[1], f[2], f[3]};	// 色
}

bool ImGuiMethod::ComboBox(const std::string& _caption, std::string& _currentItem, std::vector<const std::string*> _items)
{
	bool changed = false;

	if (ImGui::BeginCombo(_caption.c_str(), _currentItem.c_str()))
	{
		for (int n = 0; n < static_cast<int>(_items.size()); n++) {
			bool is_selected = (_currentItem == *_items[n]);
			if (ImGui::Selectable((*_items[n]).c_str(), is_selected))
			{
				_currentItem = *_items[n];
				changed = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	return changed;
}

bool ImGuiMethod::ComboBox(const std::string& _caption, u_int& _enumId, const std::vector<std::string>& _items)
{
	bool changed = false;
	std::string currentName = _items[_enumId];

	if (ImGui::BeginCombo(_caption.c_str(), currentName.c_str()))
	{
		for (int n = 0; n < static_cast<int>(_items.size()); n++) {
			bool is_selected = (_enumId == n);
			if (ImGui::Selectable(_items[n].c_str(), is_selected))
			{
				_enumId = n;
				changed = true;
			}
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}

		ImGui::EndCombo();
	}

	return changed;
}

bool ImGuiMethod::TreeNode(const std::string& _caption)
{
	return ImGui::TreeNodeEx(_caption.c_str(), ImGuiTreeNodeFlags_Framed);
}

void ImGuiMethod::EditableText(std::string& text, bool& _isEditing, u_int _id)
{
	static char buffer[IM_INPUT_BUF];        // テキスト編集用のバッファ

	if (_isEditing) {
		// テキスト入力フィールド
		std::string caption = "##editableText" + std::to_string(_id);
		if (ImGui::InputText(caption.c_str(), buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
			// Enterキーが押されたら編集を終了し、テキストを更新
			text = std::string(buffer);
			_isEditing = false;  // 編集モードを終了
		}
		if (ImGui::IsItemDeactivated()) {
			// フォーカスが外れたら編集モードを終了
			_isEditing = false;
		}
	}
	else {
		// 編集モードでない場合はラベルとして表示
		ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImGui::TextColored(color, text.c_str());
		if (ImGui::IsItemClicked()) {
			// クリックされたら編集モードに切り替え
			_isEditing = true;
			// 現在のテキストをバッファにコピー
			strcpy_s(buffer, text.c_str());
		}
	}
}

void ImGuiMethod::Text(const DirectX::SimpleMath::Vector2& _v)
{
	ImGui::Text(std::string(
		"x " + std::to_string(_v.x) +
		"\ny " + std::to_string(_v.y)
	).c_str());
}

void ImGuiMethod::Text(const Vector3& _v)
{
	ImGui::Text(std::string(
		"x " + std::to_string(_v.x) +
		"\ny " + std::to_string(_v.y) +
		"\nz " + std::to_string(_v.z)
	).c_str());
}

void ImGuiMethod::Text(const DirectX::SimpleMath::Quaternion& _q)
{
	ImGui::Text(std::string(
		"x " + std::to_string(_q.x) +
		"\ny " + std::to_string(_q.y) +
		"\nz " + std::to_string(_q.z) + 
		"\nw " + std::to_string(_q.w)
	).c_str());
}

void ImGuiMethod::SetFontSize(FontSize _size)
{
	ImGui::PushFont(pFonts[static_cast<u_int>(_size)]);
}

void ImGuiMethod::PopFontSize()
{
	ImGui::PopFont();
}

void ImGuiMethod::ImGuiLoadFonts()
{
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
}
