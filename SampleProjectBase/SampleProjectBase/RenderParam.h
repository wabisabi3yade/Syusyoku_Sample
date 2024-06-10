#pragma once

// 描画に必要なパラメータクラス
class RenderParam
{
public:
	struct WorldMatrix
	{
		DirectX::SimpleMath::Matrix world;	// ワールド座標
		DirectX::SimpleMath::Matrix view;	// ビュー変換
		DirectX::SimpleMath::Matrix projection;	// プロジェクション
	};


private:
	ID3D11InputLayout* pInputLayout;	// インプットレイアウト
	WorldMatrix worldMatrix;	// ワールド行列
public:
	RenderParam() {};
	~RenderParam();

	// インプットレイアウトを取得
	ID3D11InputLayout& GetInputLayout();
	const WorldMatrix& GetWorldMatrix() { return worldMatrix; }

	// インプットレイアウトを代入
	void SetInputLayout(ID3D11InputLayout* _inputLayout) { pInputLayout = _inputLayout; }
	void SetView(DirectX::SimpleMath::Matrix _view) { worldMatrix.view = _view; }
	void SetProjection(DirectX::SimpleMath::Matrix _proj) { worldMatrix.projection = _proj; }
};

