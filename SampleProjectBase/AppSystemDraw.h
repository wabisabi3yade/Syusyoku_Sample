#pragma once

class AssetDisplay;
class DX11BulletPhisics;

/// @brief ImGuiでシステム関連の描画を行う
class AppSystemDraw : public Singleton_Base<AppSystemDraw>, public HashiTaku::IImGuiUser
{
	friend class Singleton_Base<AppSystemDraw>;

	/// @brief 可変フレームレートクラス
	VariableFrameRate* pFrameRate;
	
public:
	void Init(VariableFrameRate& _variabeFrameRate);

private:
	AppSystemDraw();
	~AppSystemDraw() {}
	void ImGuiDebug() override;
};

