#include "pch.h"
#include "SpriteDrawer.h"

#include "Material.h"
#include "Sprite.h"

void SpriteDrawer::DrawSprite(
	const Sprite& _sprite,
	Material& _material,
	const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _scale,
	const DirectX::SimpleMath::Quaternion& _rotation
)
{
	struct TexEnable
	{
		int isTexEnable = 0;
		int dummy[3];
	};

	// レンダラー取得
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// ワールド変換行列の座標にモデルの座標を入れる
	RenderParam::WVP wvp = renderer.GetParameter().GetWVP(_pos, _scale, _rotation);
	wvp.world = wvp.world.Transpose();

	// シェーダーの設定
	VertexShader& pVs = _material.GetVertexShader();
	PixelShader& pPs = _material.GetPixelShader();
	MaterialParameter& materialParam = _material.GetMaterialParameter();
	TexEnable texEnable;
	texEnable.isTexEnable = _sprite.GetIsTexEnable();
	Texture* pTex = _sprite.GetTexture();
	pVs.UpdateSubResource(0, &wvp);
	pVs.UpdateSubResource(1, &materialParam);
	pPs.UpdateSubResource(0, &texEnable);
	pPs.SetTexture(0, pTex);
	pVs.SetGPU();
	pPs.SetGPU();
	ID3D11DeviceContext* pDeviceContext =
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	const SingleMesh& mesh = _sprite.GetSquare();
	pDeviceContext->IASetPrimitiveTopology(mesh.GetTopology());
	// バッファをGPUに送る
	mesh.GetVertexBuffer().SetGPU();
	mesh.GetIndexBuffer().SetGPU();

	pDeviceContext->DrawIndexed(
		mesh.GetIndexNum(),
		0,
		0
	);
}
