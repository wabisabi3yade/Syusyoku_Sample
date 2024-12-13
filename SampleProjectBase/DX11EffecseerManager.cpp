#include "pch.h"
#include "DX11EffecseerManager.h"
#include "InSceneSystemManager.h"
#include "CP_Camera.h"
#include "AssetGetter.h"

constexpr u_int EFFECT_DRAW_MAX(2000);	// 最大のエフェクト描画数

namespace DXSimp = DirectX::SimpleMath;

void DX11EffecseerManager::Init()
{
	pRenderer = Direct3D11::GetInstance()->GetRenderer();
	ID3D11Device& device = *pRenderer->GetDevice();
	ID3D11DeviceContext& context = *pRenderer->GetDeviceContext();

	// Effekseerの初期化を行う
	renderer = EffekseerRendererDX11::Renderer::Create(&device, &context, EFFECT_DRAW_MAX);
	manager = Effekseer::Manager::Create(EFFECT_DRAW_MAX);
	manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
	manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
	manager->SetRingRenderer(renderer->CreateRingRenderer());
	manager->SetTrackRenderer(renderer->CreateTrackRenderer());
	manager->SetModelRenderer(renderer->CreateModelRenderer());

	// テクスチャ、モデル、カーブ、マテリアルローダーの設定
	manager->SetTextureLoader(renderer->CreateTextureLoader());
	manager->SetModelLoader(renderer->CreateModelLoader());
	manager->SetMaterialLoader(renderer->CreateMaterialLoader());
	manager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void DX11EffecseerManager::BeginDraw()
{
}

void DX11EffecseerManager::EffectDraw()
{
	renderer->BeginRendering();
	UpdateCamMatrix();
	manager->Draw();
	renderer->EndRendering();
}

void DX11EffecseerManager::Update()
{
	manager->Update();
}

void DX11EffecseerManager::AllEffectDestroy()
{
	manager->StopAllEffects();
}

bool DX11EffecseerManager::ExistEffect(Effekseer::Handle _vfxHandle) const
{
	return manager->Exists(_vfxHandle);
}

Effekseer::Handle DX11EffecseerManager::Play(const Effekseer::EffectRef& _effect,
	float _playSpeed,
	const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _scale,
	const DirectX::SimpleMath::Vector3& _eularAngles,
	int _startFrame)
{
	// Effekseer側で再生を行う
	Effekseer::Vector3D pos = { _pos.x, _pos.y, _pos.z };
	Effekseer::Handle handle = manager->Play(_effect, pos, _startFrame);

	// スケール設定
	manager->SetScale(handle, _scale.x, _scale.y, _scale.z);

	// 角度
	manager->SetRotation(handle,
		_eularAngles.x * Mathf::degToRad,
		_eularAngles.y * Mathf::degToRad,
		_eularAngles.z * Mathf::degToRad);

	// 速度
	manager->SetSpeed(handle, _playSpeed);

	return  handle;
}

Effekseer::Handle DX11EffecseerManager::Play(const CreateVfxInfo& _createVfx, const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _eularAngles)
{
	return Play(_createVfx.pHitVfx->GetEffect(),
		_createVfx.speed,
		_pos,
		_createVfx.scale * DXSimp::Vector3::One,
		_eularAngles,
		_createVfx.startFrame);
}

const Effekseer::ManagerRef& DX11EffecseerManager::GetManager() const
{
	return manager;
}

DX11EffecseerManager::DX11EffecseerManager() : pRenderer(nullptr)
{
}

DX11EffecseerManager::~DX11EffecseerManager()
{
}

void DX11EffecseerManager::UpdateCamMatrix()
{
	InSceneSystemManager* pInSceneSystem = InSceneSystemManager::GetInstance();

	//転置済みなので転置して元に戻す

	// プロジェクション行列
	Effekseer::Matrix44 projMtx;
	CreateEffekseerMtx(pRenderer->GetParameter().GetWVP().projection.Transpose(),
		projMtx);
	renderer->SetProjectionMatrix(projMtx);

	// カメラ行列
	CP_Camera& camera = pInSceneSystem->GetMainCamera();
	renderer->SetCameraMatrix(
		Effekseer::Matrix44().LookAtLH(
			CreateEffekseerVector3(camera.GetEyePos()),
			CreateEffekseerVector3(camera.GetTarget()),
			CreateEffekseerVector3(camera.GetCamUp())
		)
	);
}

void DX11EffecseerManager::CreateEffekseerMtx(const DirectX::SimpleMath::Matrix& _dxMtx, Effekseer::Matrix44& _outMtx)
{
	_outMtx.Values[0][0] = _dxMtx._11;
	_outMtx.Values[0][1] = _dxMtx._12;
	_outMtx.Values[0][2] = _dxMtx._13;
	_outMtx.Values[0][3] = _dxMtx._14;

	_outMtx.Values[1][0] = _dxMtx._21;
	_outMtx.Values[1][1] = _dxMtx._22;
	_outMtx.Values[1][2] = _dxMtx._23;
	_outMtx.Values[1][3] = _dxMtx._24;

	_outMtx.Values[2][0] = _dxMtx._31;
	_outMtx.Values[2][1] = _dxMtx._32;
	_outMtx.Values[2][2] = _dxMtx._33;
	_outMtx.Values[2][3] = _dxMtx._34;

	_outMtx.Values[3][0] = _dxMtx._41;
	_outMtx.Values[3][1] = _dxMtx._42;
	_outMtx.Values[3][2] = _dxMtx._43;
	_outMtx.Values[3][3] = _dxMtx._44;
}

Effekseer::Vector3D DX11EffecseerManager::CreateEffekseerVector3(const DirectX::SimpleMath::Vector3& _dxVec3)
{
	return Effekseer::Vector3D(_dxVec3.x, _dxVec3.y, _dxVec3.z);
}

nlohmann::json CreateVfxInfo::Save()
{
	nlohmann::json vfxData;

	if (pHitVfx)
		vfxData["name"] = pHitVfx->GetAssetName();

	vfxData["scale"] = scale;
	vfxData["speed"] = speed;
	vfxData["startFrame"] = startFrame;

	return vfxData;
}

void CreateVfxInfo::Load(const nlohmann::json& _data)
{
	std::string vfxName;
	if (HashiTaku::LoadJsonString("name", vfxName, _data))
	{
		pHitVfx = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}

	HashiTaku::LoadJsonFloat("scale", scale, _data);
	HashiTaku::LoadJsonFloat("speed", speed, _data);
	HashiTaku::LoadJsonInteger("startFrame", startFrame, _data);
}

void CreateVfxInfo::ImGuiDebug()
{
#ifdef EDIT
	if (!ImGuiMethod::TreeNode("Vfx Info")) return;

	// ヒット時エフェクト
	if (ImGui::Button("Clear"))
		pHitVfx = nullptr;
	std::string vfxName = "Null";
	if (pHitVfx)
		vfxName = pHitVfx->GetAssetName();
	if (AssetGetter::ImGuiGetCombobox<VisualEffect>("HitVfx", vfxName))
	{
		pHitVfx = AssetGetter::GetAsset<VisualEffect>(vfxName);
	}

	// 各パラメータ
	ImGui::DragFloat("Scale", &scale, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 1000.0f);
	ImGui::DragInt("StartFrame", &startFrame, 1.0f, 0, 200);

	ImGui::TreePop();
#endif
}
