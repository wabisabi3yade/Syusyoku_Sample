#pragma once

class AssetDisplay;
class DX11BulletPhisics;

/// @brief ImGuiでシステム関連の描画を行う
class AppSystemDraw : public HashiTaku::IImGuiUser
{
	/// @brief 可変フレームレートクラス
	VariableFrameRate* pFrameRate;

	/// @brief あたり判定
	DX11BulletPhisics* pBulletEngine;
	
public:
	AppSystemDraw(VariableFrameRate& _frameRate, DX11BulletPhisics& _bulletEngine);
	~AppSystemDraw() {}

private:
	void ImGuiSetting() override;
};

