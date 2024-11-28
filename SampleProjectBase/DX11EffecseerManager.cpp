#include "pch.h"
#include "DX11EffecseerManager.h"
#include "InSceneSystemManager.h"
#include "CP_Camera.h"

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
	const DirectX::SimpleMath::Vector3& _pos,
	const DirectX::SimpleMath::Vector3& _scale)
{
	Effekseer::Handle handle = manager->Play(_effect, 0.0f, 0.0f, 0.0f);

	// 座標・スケール反映
	Effekseer::Matrix43 transformMatrix;
	transformMatrix.Value[0][0] = _scale.x;
	transformMatrix.Value[1][1] = _scale.y;
	transformMatrix.Value[2][2] = _scale.z;
	transformMatrix.Value[3][0] = _pos.x;
	transformMatrix.Value[3][1] = _pos.y;
	transformMatrix.Value[3][2] = _pos.z;
	manager->SetMatrix(handle, transformMatrix);

	return  handle;
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
