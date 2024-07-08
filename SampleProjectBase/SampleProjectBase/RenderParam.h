#pragma once

// 描画に必要なパラメータクラス
class RenderParam
{
public:
	struct WVP	// ワールド・ビュー・プロジェクション	
	{
		DirectX::SimpleMath::Matrix world;	// ワールド変換行列
		DirectX::SimpleMath::Matrix view;	// ビュー変換
		DirectX::SimpleMath::Matrix projection;	// プロジェクション
	};

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;	// インプットレイアウト
	WVP wvp;	// ワールド行列
public:
	RenderParam() : pInputLayout(nullptr) {};
	~RenderParam() {};

	// インプットレイアウトを取得
	ID3D11InputLayout& GetInputLayout();
	const WVP& GetWVP() { return wvp; }	// WVPを取得

	// インプットレイアウトを代入
	void SetInputLayout(ID3D11InputLayout* _inputLayout) { pInputLayout = _inputLayout; }
	void SetView(DirectX::SimpleMath::Matrix _view) { wvp.view = _view; }
	void SetProjection(DirectX::SimpleMath::Matrix _proj) { wvp.projection = _proj; }
};

