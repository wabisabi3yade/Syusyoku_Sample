#include "pch.h"
#include "Sphere.h"

#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
void Sphere::Make()
{
	std::vector<Vertex> verticies;
	const int detail = 32;
	for (int j = 0; j < detail / 2 + 1; ++j)
	{
		float radY = DirectX::XM_2PI / detail * j;
		for (int i = 0; i < detail; ++i)
		{
			float radXZ = DirectX::XM_2PI / detail * i;
			float x = sinf(radY) * sinf(radXZ);
			float y = cosf(radY);
			float z = sinf(radY) * cosf(radXZ);
			Vertex vertex;
			vertex.position = Vector3(x * 0.5f, y * 0.5f, z * 0.5f);
			vertex.color = Color(1.0f, 1.0f, 1.0f, 1.0f);
			vertex.normal = vertex.position;
			vertex.normal.Normalize();
			vertex.uv = Vector2::Zero;
			
			verticies.push_back(vertex);
		}
	}

	std::vector<u_int> indicies;
	for (int j = 0; j < detail / 2; ++j)
	{
		for (int i = 0; i < detail; ++i)
		{
			indicies.push_back((j + 1) * detail + i);
			indicies.push_back(j * detail + i);
			indicies.push_back(j * detail + ((i + 1) % detail));
			indicies.push_back(indicies.back());
			indicies.push_back((j + 1) * detail + ((i + 1) % detail));
			indicies.push_back((j + 1) * detail + i);
		}
	}

	// 絶対にベクター配列にするけど時間がないから処理してるゾーン //
	vertexNum = (u_int)verticies.size();
	pVertices = new Vertex[vertexNum];
	for (u_int i = 0; i < vertexNum; i++)
	{
		pVertices[i] = verticies[i];
	}

	indexNum = (u_int)indicies.size();
	pIndicies = new u_int[indexNum];
	for (u_int i = 0; i < indexNum; i++)
	{
		pIndicies[i] = indicies[i];
	}
	// ゾーン終了 //

	// バッファ作成
	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();
	CreateVertexBuffer(*renderer);
	CreateIndexBuffer(*renderer);

}

Sphere::Sphere()
{
	Make();
}

Sphere::~Sphere()
{
}

void Sphere::Draw(const Transform& _transform)
{

	D3D11_Renderer* renderer = Direct3D11::GetInstance()->GetRenderer();

	renderer->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ワールド変換行列を取得
	Matrix worldmtx = D3D11_Renderer::GetWorldMtx(_transform);

	// ワールド変換行列をWVPに入れる
	RenderParam::WVP wMat = renderer->GetParameter().GetWVP();
	wMat.world = worldmtx;

	// シェーダーをセット
	pMaterial->GetVertexShader().Bind();
	pMaterial->GetPixelShader().Bind();

	// シェーダーにバッファを送る
	pMaterial->GetVertexShader().UpdateBuffer(0, &wMat);
	MaterialParameter& materialParam = pMaterial->GetMaterialParameter();
	pMaterial->GetVertexShader().UpdateBuffer(1, &materialParam);

	// ディレクションライトをバッファにセットして送る
	SceneLights& pSceneLights =  InSceneSystemManager::GetInstance()->GetSceneLights();
	DirectionLParameter dirParam = pSceneLights.GetDirectionParameter();
	pMaterial->GetVertexShader().UpdateBuffer(2, &dirParam);

	pMaterial->GetPixelShader().UpdateBuffer(0, &materialParam);

	

	Mesh::Draw(*renderer);
}

void Sphere::DebugDraw(Transform& _transform, DirectX::SimpleMath::Color& _color, bool _isWireFrame)
{
	// トポロジーを設定
	ID3D11DeviceContext* pContext = Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

	D3D_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	if (_isWireFrame)
		topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;

	pContext->IASetPrimitiveTopology(topology);

	BasicObject_Base::BasicDraw(_transform, _color);
}
