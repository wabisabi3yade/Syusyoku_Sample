#pragma once
#include "CP_Renderer.h"

// 描画に必要
#include "Mesh_Group.h"

// メッシュ描画コンポーネント
class CP_MeshRenderer : public CP_Renderer
{
	/// @brief 描画するメッシュ群
	Mesh_Group* pRenderMesh{ nullptr };

	/// @brief オフセット座標
	DirectX::SimpleMath::Vector3 offsetPos;

	/// @brief オフセット角度
	DirectX::SimpleMath::Vector3 offsetAngles;

	/// @brief オブジェクトの原点を表示するか
	bool isOriginDisplay;
public:
	CP_MeshRenderer();
	~CP_MeshRenderer() {}

	void Init();

	void Draw() override;

	void ImGuiSetting() override;

	/// @brief 描画するメッシュをセット
	/// @param _renderMesh メッシュ群
	void SetRenderMesh(Mesh_Group& _renderMesh);

	/// @brief 描画するメッシュをセット
	/// @param _assetName メッシュ名
	void SetRenderMesh(const std::string& _assetName);
	
	// シェーダーの名前からシェーダーを変更する
	void SetVertexShader(const std::string& _vsName);
	void SetPixelShader(const std::string& _psName);

	// オフセット座標をセット
	void SetOffsetPos(const DirectX::SimpleMath::Vector3& _offset);

	// オフセット角度をセット(deg)
	void SetOffsetAngle(const DirectX::SimpleMath::Vector3& _offset);

	/// @brief 描画するメッシュを取得
	/// @return メッシュ群
	Mesh_Group* GetRenderMesh();

private:
	/// @brief 描画できるのか返す
	/// @return 描画できるか？
	bool IsCanDraw();

	/// @brief メッシュを描画
	void DrawMesh(RenderParam::WVP _wvp);

	/// @brief マテリアルの準備
	/// @param _wvp wvp行列
	/// @param _mtrlIdx　マテリアルID
	void MaterialSetup(RenderParam::WVP& _wvp, u_int _mtrlIdx);

	/// @brief シェーダーの準備
	/// @param _shader シェーダー
	/// @param _wvp wvp行列
	/// @param _material マテリアル
	void ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material);

	/// @brief ローカル軸を考慮したオフセットに変換
	/// @param _worldOffset 変換したいオフセット
	DirectX::SimpleMath::Vector3 WorldOffset(const DirectX::SimpleMath::Vector3& _offset);

	/// @brief 原点表示
	void OriginDisplay();
};

