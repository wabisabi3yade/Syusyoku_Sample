#include "pch.h"
#include "ImGuiMethod.h"

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui.h"

#include "ImGuiDebugLog.h"

using namespace DirectX::SimpleMath;

void ImGuiMethod::Initialize(HWND _hwnd, ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImWchar const ranges[] = { 0x0020, 0xfffd, 0, };
	io.Fonts->AddFontFromFileTTF("assets/font/MSGOTHIC.TTF", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls


	// ImGuiのスタイルを設定
	ImGui::StyleColorsClassic();

	// ImGuiのバックエンドを初期化
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(_pDevice, _pDeviceContext);
}

void ImGuiMethod::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiMethod::Draw()
{
	ImGuiDebugLog::DisplayMessage();

	// ImGui　描画
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiMethod::End()
{
	// デバッグログ終了処理
	ImGuiDebugLog::Terminate();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiMethod::DragFloat3(Vector3& _vector3, const std::string& _name)
{
	static float f[3];
	f[0] = _vector3.x;
	f[1] = _vector3.y;
	f[2] = _vector3.z;
	ImGui::DragFloat3(ShiftJisToUtf8(_name).c_str(), f);
	_vector3 = { f[0], f[1], f[2] };	// 座標
}

void ImGuiMethod::ColorEdit4(DirectX::SimpleMath::Color& _color, const std::string& _name)
{
	static float f[4];
	f[0] = _color.x;
	f[1] = _color.y;
	f[2] = _color.z;
	f[3] = _color.w;
	ImGui::ColorEdit4(ShiftJisToUtf8(_name).c_str(), f);
	_color = { f[0], f[1], f[2], f[3]};	// 色
}
