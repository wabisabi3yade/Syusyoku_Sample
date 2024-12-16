#include "pch.h"
#include "Geometory.h"

// ���b�V��
#include "StaticMesh.h"
#include "Cube.h"
#include "DebugCube.h"
#include "Sphere.h"

// �V�F�[�_�[
#include "ShaderCollection.h"

// �A�Z�b�g
#include "AssetSetter.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	using namespace DXSimp;

	Material* Geometory::pMaterial = nullptr;
	std::vector<StaticMesh*> Geometory::pGeometory = {};
	Vector3 Geometory::position = Vector3::Zero;
	Vector3 Geometory::scale = Vector3::One;
	Vector3 Geometory::eularAngle = Vector3::Zero;
	Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	std::vector<Geometory::LineVertex>  Geometory::drawLines;
	VertexShader* Geometory::pLineVs = nullptr;
	PixelShader* Geometory::pLinePs = nullptr;

	void Geometory::DrawSetup()
	{
		// �����_���[�擾
		D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

		// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
		Quaternion q = Quat::ToQuaternion(eularAngle);
		RenderParam::WVP wvp = renderer.GetParameter().GetWVP(position, scale, q);
		wvp.world = wvp.world.Transpose();

		// �V�F�[�_�[�̐ݒ�
		VertexShader& pVs = pMaterial->GetVertexShader();
		PixelShader& pPs = pMaterial->GetPixelShader();

		pVs.UpdateSubResource(0, &wvp);
		pVs.UpdateSubResource(1, &color);

		pVs.SetGPU();
		pPs.SetGPU();
	}

	void Geometory::Draw(StaticMesh& _staticMesh)
	{
		ID3D11DeviceContext* pDeviceContext =
			Direct3D11::GetInstance()->GetRenderer()->GetDeviceContext();

		const SingleMesh& singleMesh = *_staticMesh.GetMesh(0);

		// �g�|���W�[�ݒ�
		pDeviceContext->IASetPrimitiveTopology(singleMesh.GetTopology());

		// �o�b�t�@��GPU�ɑ���
		singleMesh.GetVertexBuffer().SetGPU();
		singleMesh.GetIndexBuffer().SetGPU();

		pDeviceContext->DrawIndexed(
			singleMesh.GetIndexNum(),
			0,
			0
		);
	}

	void Geometory::Init()
	{
		// �}�e���A�����쐬
		MakeMaterial();

		// �L���[�u���쐬
		MakeGeometory();

		// �V�F�[�_�[�擾
		GetShader();
	}

	void Geometory::Release()
	{
	}

	void Geometory::DrawCube(bool _isWireFrame)
	{
		DrawSetup();

		int geoType = static_cast<int>(GeoType::Cube);

		if (_isWireFrame)
			geoType = static_cast<int>(GeoType::WireCube);

		Draw(*pGeometory[geoType]);

		// ���ɖ߂�
		ResetParameter();
	}

	void Geometory::DrawSphere(bool _isWireFrame)
	{
		DrawSetup();

		int geoType = static_cast<int>(GeoType::Sphere);
		Draw(*pGeometory[geoType]);

		// ���ɖ߂�
		ResetParameter();
	}

	void Geometory::AddLine(const DirectX::SimpleMath::Vector3& _start, const DirectX::SimpleMath::Vector3& _end, const DirectX::SimpleMath::Color& _color)
	{
		LineVertex line;
		line.position = _start;
		line.color = _color;
		drawLines.push_back(line);

		line.position = _end;
		drawLines.push_back(line);
	}

	void Geometory::DrawLine()
	{
		if (drawLines.size() <= 0) return;

		D3D11_Renderer* pRenderer = Direct3D11::GetInstance()->GetRenderer();
		ID3D11Device* pDevice = pRenderer->GetDevice();
		ID3D11DeviceContext* pDeviceContext = pRenderer->GetDeviceContext();

		// �g�|���W�[�ݒ�
		pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		// ���_�o�b�t�@�쐬
		u_int vertexCnt = static_cast<int>(drawLines.size());
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(LineVertex) * vertexCnt;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = drawLines.data();
		ID3D11Buffer* pVertexBuffer = nullptr;
		pDevice->CreateBuffer(&bufferDesc, &initData, &pVertexBuffer);

		// ���[���h�ϊ��s��̍��W�Ƀ��f���̍��W������
		RenderParam::WVP& wvp = pRenderer->GetParameter().GetWVP(Vector3::Zero, Vector3::One, Quaternion::Identity);
		wvp.world = wvp.world.Transpose();

		// �V�F�[�_�[�̐ݒ�
		pLineVs->UpdateSubResource(0, &wvp);
		pLineVs->SetGPU();
		pLinePs->SetGPU();

		// �o�b�t�@�𑗂�
		UINT stride = sizeof(LineVertex);
		UINT offset = 0;
		pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);

		SAFE_RELEASE(pVertexBuffer);

		// ���ۂ̕`��
		pDeviceContext->Draw(vertexCnt, 0);

		drawLines.clear();
	}

	void Geometory::MakeMaterial()
	{
		// �}�e���A���쐬
		const std::string MATERIAL_NAME = "M_Geometory";
		std::unique_ptr<Material> makeMaterial = std::make_unique<Material>("VS_Primitive", "PS_Primitive");
		makeMaterial->SetIsntSave();

		// ���̃I�u�W�F�N�g�Ƀ}�e���A���Z�b�g
		pMaterial = AssetSetter::SetAsset<Material>(MATERIAL_NAME, std::move(makeMaterial));
	}

	void Geometory::MakeGeometory()
	{
		// �쐬���郁�b�V��
		std::vector<std::unique_ptr<SingleMesh>> pMeshes;

		pMeshes.push_back(std::make_unique<Cube>());
		pMeshes.push_back(std::make_unique<DebugCube>());
		pMeshes.push_back(std::make_unique<Sphere>());

		// ���O
		std::vector<std::string> names =
		{
			"SM_Cube",
			"SM_WireCube",
			"SM_Sphere"
		};

		Material* pUnlit = AssetGetter::GetAsset<Material>("M_Unlit");

		// �A�Z�b�g�Ǘ��ɃZ�b�g����
		for (u_int loop = 0; loop < static_cast<u_int>(pMeshes.size()); loop++)
		{
			std::unique_ptr<StaticMesh> pSM = std::make_unique<StaticMesh>();

			// ���b�V���ƃ}�e���A�����Z�b�g
			pSM->AddMesh(std::move(pMeshes[loop]));
			pSM->AddMaterial(pUnlit);
			pSM->SetIsntSave();

			// �A�Z�b�g�Ǘ��ɃZ�b�g
			pGeometory.push_back(AssetSetter::SetAsset(names[loop], std::move(pSM)));
		}
	}

	void Geometory::GetShader()
	{
		ShaderCollection* shCol = ShaderCollection::GetInstance();
		pLineVs = shCol->GetVertexShader("VS_Line");
		pLinePs = shCol->GetPixelShader("PS_Line");

		assert(pLineVs && pLinePs && "���`��̃V�F�[�_�[�𐳏�Ɏ擾�ł��܂���");
	}

	void Geometory::ResetParameter()
	{
		position = Vector3::Zero;
		scale = Vector3::One;
		eularAngle = Vector3::Zero;
		color = Color(1.f, 1.f, 1.f, 1.f);
	}
}