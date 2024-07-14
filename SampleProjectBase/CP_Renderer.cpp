#include "pch.h"
#include "CP_Renderer.h"

// メッシュ
#include "SingleMesh.h"

void CP_Renderer::DrawMesh(const SingleMesh& _mesh)
{
	ID3D11DeviceContext* pDeviceContext = 
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// トポロジー設定
	pDeviceContext->IASetPrimitiveTopology(_mesh.GetTopology());

	// バッファをGPUに送る
	_mesh.GetVertexBuffer().SetGPU();
	_mesh.GetIndexBuffer().SetGPU();

	pDeviceContext->DrawIndexed(
		_mesh.GetIndexNum(),
		0,
		0
	);
}
