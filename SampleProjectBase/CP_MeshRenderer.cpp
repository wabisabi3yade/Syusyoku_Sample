#include "pch.h"
#include "CP_MeshRenderer.h"

// システム
#include "AssetGetter.h"
#include "InSceneSystemManager.h"
#include "ShaderCollection.h"

#include "Geometory.h"

namespace HashiTaku
{
	constexpr u_int TEX_DIFUSSE_SLOT(0);	// ディフューズテクスチャのスロット
	constexpr float ORIGIN_SCALE(0.15f);	// 原点表示のオブジェクトのスケール
	constexpr DXSimp::Color ORIGIN_COLOR(1.0f, 1.0f, 0.0f);	// 原点表示のオブジェクトの色

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

		// WVP求める
		Transform& transform = GetTransform();
		auto& wvp = rendererParam.GetWVP();
		wvp.world = CalcLoadMtx() * transform.GetWorldMatrix();
		wvp.world = wvp.world.Transpose();

		// メッシュ描画
		DrawMesh(wvp);

		// 原点表示
		OriginDisplay();
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


	void CP_MeshRenderer::ImGuiDebug()
	{
#ifdef EDIT
		// ImGui開いたときだけ原点表示
		isOriginDisplay = true;

		// メッシュ取得
		std::string meshName;
		if (pRenderMesh)
			meshName = pRenderMesh->GetAssetName();
		if (AssetGetter::ImGuiGetCombobox<Mesh_Group>("model", meshName))
		{
			pRenderMesh = AssetGetter::GetAsset<Mesh_Group>(meshName);
		}
		ImGuiMethod::LineSpaceSmall();

		ImGui::Text("Material Set");
		// マテリアルを取得
		u_int cnt = 0;
		auto endItr = setMaterials.end();
		for (auto matItr = setMaterials.begin(); matItr != endItr; ++matItr)
		{
			// テキスト
			std::string display = std::to_string(cnt) + ":";
			std::string matName = "Null";
			if (*matItr != nullptr)
				matName = (*matItr)->GetAssetName();

			display += matName;

			ImGui::Text(display.c_str());

			ImGui::SameLine();

			// Nullにする
			if (ImGui::Button("Null"))
				*matItr = nullptr;

			cnt++;
		}
		std::string meshCntText = "meshCnt:" + static_cast<u_int>(setMaterials.size());
		ImGui::Text(meshCntText.c_str());
		if (ImGui::Button("+"))
		{
			setMaterials.push_back(nullptr);	// 枠追加
		}
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			setMaterials.pop_back();	// 枠追加
		}


		ImGuiMethod::LineSpaceSmall();

		int useMaterialCnt = static_cast<int>(setMaterials.size());

		// マテリアルを指定
		ImGuiMethod::PushItemSmallWidth();
		static int inputId = 0;
		ImGui::DragInt("MatId", &inputId, 1, 0, useMaterialCnt - 1);
		ImGui::PopItemWidth();

		// アセットからマテリアル選ぶ
		static std::string getName;
		AssetGetter::ImGuiGetCombobox<Material>("##material", getName);

		// セットする
		if (ImGui::Button("Set"))
		{
			if (inputId >= useMaterialCnt) return;

			// マテリアルを取得
			Material* pGetMat = AssetGetter::GetAsset<Material>(getName);
			if (!pGetMat) return;

			// セット
			auto setItr = std::next(setMaterials.begin(), inputId);
			(*setItr) = pGetMat;
		}

#endif // EDIT
	}


	Mesh_Group* CP_MeshRenderer::GetRenderMesh()
	{
		return pRenderMesh;
	}

	json CP_MeshRenderer::Save()
	{
		auto data = CP_Renderer::Save();

		if (pRenderMesh)
			data["meshName"] = pRenderMesh->GetAssetName();

		return data;
	}

	void CP_MeshRenderer::Load(const json& _data)
	{
		CP_Renderer::Load(_data);

		pRenderMesh = LoadJsonAsset<Mesh_Group>("meshName", _data);
	}

	bool CP_MeshRenderer::IsCanDraw()
	{
		if (pRenderMesh == nullptr) return false;

		return true;
	}

	DXSimp::Matrix CP_MeshRenderer::CalcLoadMtx()
	{
		using namespace DXSimp;

		float loadScale = pRenderMesh->GetLoadOffsetScale();
		Vector3 loadAngles = pRenderMesh->GetLoadOffsetAngles() * Mathf::degToRad;

		return Matrix::CreateScale(Vector3::One * loadScale) *
			Matrix::CreateFromYawPitchRoll(loadAngles.y, loadAngles.x, loadAngles.z);
	}

	void CP_MeshRenderer::DrawMesh(RenderParam::WVP& _wvp)
	{
		// メッシュ数
		u_int meshCnt = pRenderMesh->GetMeshNum();

		// このレンダラーでセットされているマテリアル数
		u_int rendererMatCnt = static_cast<u_int>(setMaterials.size());

		auto setMatItr = setMaterials.begin();

		for (u_int meshLoop = 0; meshLoop < meshCnt; meshLoop++)
		{
			// メッシュを取得
			const SingleMesh* pSingleMesh = pRenderMesh->GetMesh(meshLoop);

			// 今回描画するマテリアル
			Material* pRenderMaterial = nullptr;

			if (meshLoop < rendererMatCnt)	// セットされているマテリアルを取得
			{
				pRenderMaterial = *setMatItr;
				++setMatItr;
			}

			// まだセットされていないなら
			if (!pRenderMaterial)
			{
				// メッシュ側から取得
				u_int materialID = pSingleMesh->GetMaterialID();
				pRenderMaterial = pRenderMesh->GetMaterial(materialID);
			}

			// マテリアルの描画準備
			MaterialSetup(_wvp, pRenderMaterial);

			// メッシュ描画
			CP_Renderer::DrawMesh(*pSingleMesh);
		}
	}

	void CP_MeshRenderer::DrawShadow()
	{

	}

	void CP_MeshRenderer::MaterialSetup(RenderParam::WVP& _wvp, Material* _pMaterial)
	{
		if (_pMaterial == nullptr) return;

		VertexShader& pVS = _pMaterial->GetVertexShader();
		PixelShader& pPs = _pMaterial->GetPixelShader();

		// シェーダーにバッファを送る
		// (ここではライト、カメラ座標などの1ループで1度しか送らないものは送らない)
		ShaderSetup(pVS, _wvp, *_pMaterial);
		ShaderSetup(pPs, _wvp, *_pMaterial);

		// テクスチャを送る
		pPs.SetTexture(TEX_DIFUSSE_SLOT, _pMaterial->GetDiffuseTexture());

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

	DXSimp::Vector3 CP_MeshRenderer::WorldOffset(const DXSimp::Vector3& _offset)
	{
		DXSimp::Vector3 worldOffset = _offset.x * GetTransform().Right();
		worldOffset += _offset.y * GetTransform().Up();
		worldOffset += _offset.z * GetTransform().Forward();

		return worldOffset;
	}

	void CP_MeshRenderer::OriginDisplay()
	{
#ifdef EDIT
		if (!isOriginDisplay) return;

		Geometory::SetPosition(GetTransform().GetPosition());
		Geometory::SetScale(ORIGIN_SCALE * DXSimp::Vector3::One);
		Geometory::SetColor(ORIGIN_COLOR);
		Geometory::DrawCube();

		isOriginDisplay = false;
#endif // EDIT
	}
}