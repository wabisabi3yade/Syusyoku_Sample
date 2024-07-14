#include "pch.h"
#include "CP_Renderer.h"

// ���b�V��
#include "SingleMesh.h"

void CP_Renderer::DrawMesh(const SingleMesh& _mesh)
{
	ID3D11DeviceContext* pDeviceContext = 
		Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	// �g�|���W�[�ݒ�
	pDeviceContext->IASetPrimitiveTopology(_mesh.GetTopology());

	// �o�b�t�@��GPU�ɑ���
	_mesh.GetVertexBuffer().SetGPU();
	_mesh.GetIndexBuffer().SetGPU();

	pDeviceContext->DrawIndexed(
		_mesh.GetIndexNum(),
		0,
		0
	);
}
