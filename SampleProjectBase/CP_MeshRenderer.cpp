#include "pch.h"
#include "CP_MeshRenderer.h"

// システム
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

#include "Geometory.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

constexpr u_int TEX_DIFUSSE_SLOT(0);	// ディフューズテクスチャのスロット
constexpr float ORIGIN_SCALE(0.15f);	// 原点表示のオブジェクトのスケール
constexpr Color ORIGIN_COLOR(1.0f, 1.0f, 0.0f);	// 原点表示のオブジェクトの色

CP_MeshRenderer::CP_MeshRenderer()
	: pRenderMesh{ nullptr }, isOriginDisplay(false)
{
}

void CP_MeshRenderer::Start()
{
}

void CP_MeshRenderer::Draw()
{
	if (!IsCanDraw()) return;

	RenderParam& rendererParam = Direct3D11::GetInstance()->GetRenderer()->GetParameter();

	Transform& transform = GetTransform();
	auto& wvp = rendererParam.GetWVP(transform);

	wvp.world = CalcLoadMtx() * wvp.world;
	wvp.world = wvp.world.Transpose();

	// メッシュ描画
	DrawMesh(wvp);

	// 原点表示
	OriginDisplay();

	// 影描画
	/*Matrix mtx[3];
	mtx[0] = Matrix::Identity;
	mtx[0] = mtx[0] * Matrix::CreateScale(scale);

	Matrix LMat[3];
	LMat[0] = mtx[0];
	SceneLights& sceneLight = InSceneSystemManager::GetInstance()->GetSceneLights();
	SceneLightsParam& lightParam = sceneLight.GetLightsParam();
	Vector3 lightPos = lightParam.dirParam.base.position;
	Vector3 lightDir = lightParam.dirParam.direction;

	LMat[1] = DirectX::XMMatrixLookAtLH(
		DirectX::XMVectorSet(lightPos.x, lightPos.y, lightPos.z, 0.0f),
		DirectX::XMVectorSet(lightPos.x + lightDir.x, lightPos.y + lightDir.y, lightPos.z + lightDir.z, 0.0f),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);
	LMat[1].Transpose(LMat[1]);

	LMat[2] = DirectX::XMMatrixOrthographicLH(5.0f, 5.0f, 0.1f, 100.0f);
	LMat[2].Transpose(LMat[2]);

	Color color = { 1.0f,1.0f,1.0f,1.0f };*/
}

void CP_MeshRenderer::SetRenderMesh(Mesh_Group& _renderMesh)
{
	pRenderMesh = &_renderMesh;
}

void CP_MeshRenderer::SetRenderMesh(const std::string& _assetName)
{
	pRenderMesh = AssetGetter::GetAsset<Mesh_Group>(_assetName);
}

void CP_MeshRenderer::SetVertexShader(const std::string& _vsName)
{
	assert(pRenderMesh != nullptr);

	pRenderMesh->SetVertexShader(_vsName);
}

void CP_MeshRenderer::SetPixelShader(const std::string& _psName)
{
	assert(pRenderMesh != nullptr);

	pRenderMesh->SetPixelShader(_psName);
}


void CP_MeshRenderer::ImGuiSetting()
{
	// ImGui開いたときだけ原点表示
	isOriginDisplay = true;

	constexpr u_int Buf = 256;
	static char str[Buf] = "";

	ImGui::InputText("name", str, Buf);

	if (ImGui::Button("Set"))
	{
		pRenderMesh = AssetGetter::GetAsset<Mesh_Group>(str);
	}
}


Mesh_Group* CP_MeshRenderer::GetRenderMesh()
{
	return pRenderMesh;
}

nlohmann::json CP_MeshRenderer::Save()
{
	auto data = CP_Renderer::Save();

	if (pRenderMesh)
		data["meshName"] = pRenderMesh->GetAssetName();

	return data;
}

void CP_MeshRenderer::Load(const nlohmann::json& _data)
{
	CP_Renderer::Load(_data);

	pRenderMesh = HashiTaku::LoadJsonAsset<Mesh_Group>("meshName", _data);
}

bool CP_MeshRenderer::IsCanDraw()
{
	if (pRenderMesh == nullptr) return false;

	return true;
}

DirectX::SimpleMath::Matrix CP_MeshRenderer::CalcLoadMtx()
{
	float loadScale = pRenderMesh->GetLoadOffsetScale();
	Vector3 loadAngles = pRenderMesh->GetLoadOffsetAngles() * Mathf::degToRad;

	return Matrix::CreateScale(Vector3::One * loadScale) * 
		Matrix::CreateFromYawPitchRoll(loadAngles.y, loadAngles.x, loadAngles.z);
}

void CP_MeshRenderer::DrawMesh(RenderParam::WVP& _wvp)
{
	u_int meshNum = pRenderMesh->GetMeshNum();

	for (u_int meshLoop = 0; meshLoop < meshNum; meshLoop++)
	{
		// メッシュを取得
		const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

		// 使用するマテリアル取得
		u_int materialID = pSingleMesh->GetMaterialID();

		// マテリアルの描画準備
		MaterialSetup(_wvp, materialID);

		CP_Renderer::DrawMesh(*pSingleMesh);
	}
}

void CP_MeshRenderer::MaterialSetup(RenderParam::WVP& _wvp, u_int _mtrlIdx)
{
	Material* pMaterial = pRenderMesh->GetMaterial(_mtrlIdx);
	if (pMaterial == nullptr) return;

	VertexShader& pVS = pMaterial->GetVertexShader();
	PixelShader& pPs = pMaterial->GetPixelShader();

	// シェーダーにバッファを送る
	// (ここではライト、カメラ座標などの1ループで1度しか送らないものは送らない)
	ShaderSetup(pVS, _wvp, *pMaterial);
	ShaderSetup(pPs, _wvp, *pMaterial);

	// テクスチャを送る
	pPs.SetTexture(TEX_DIFUSSE_SLOT, pMaterial->GetDiffuseTexture());

	// GPUに送る
	pVS.SetGPU();
	pPs.SetGPU();
}

void CP_MeshRenderer::ShaderSetup(Shader& _shader, RenderParam::WVP& _wvp, Material& _material)
{
	// バッファの種類からスロットの番号に送る
	u_int bufferNum = _shader.GetBufferNum();

	using enum Shader::BufferType;

	for (u_int bufLoop = 0; bufLoop < bufferNum; bufLoop++)
	{
		switch (_shader.GetBufferType(bufLoop))
		{
		case WVP:	// WVP行列
			_shader.UpdateSubResource(bufLoop, &_wvp);
			break;

		case Material:	// マテリアル
			_shader.UpdateSubResource(bufLoop, &_material.GetMaterialParameter());
			break;

		default:
			break;
		}
	}
}

DirectX::SimpleMath::Vector3 CP_MeshRenderer::WorldOffset(const DirectX::SimpleMath::Vector3& _offset)
{
	Vector3 worldOffset = _offset.x * GetTransform().Right();
	worldOffset += _offset.y * GetTransform().Up();
	worldOffset += _offset.z * GetTransform().Forward();

	return worldOffset;
}

void CP_MeshRenderer::OriginDisplay()
{
#ifdef EDIT
	if (!isOriginDisplay) return;

	Geometory::SetPosition(GetTransform().GetPosition());
	Geometory::SetScale(ORIGIN_SCALE * Vector3::One);
	Geometory::SetColor(ORIGIN_COLOR);
	Geometory::DrawCube();

	isOriginDisplay = false;
#endif // EDIT
}
