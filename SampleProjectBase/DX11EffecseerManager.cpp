#include "pch.h"
#include "DX11EffecseerManager.h"
#include "InSceneSystemManager.h"
#include "CP_Camera.h"
#include "AssetGetter.h"

namespace HashiTaku
{
	constexpr u_int EFFECT_DRAW_MAX(5000);	// �ő�̃G�t�F�N�g�`�搔

	void DX11EffecseerManager::Init()
	{
		pRenderer = Direct3D11::GetInstance()->GetRenderer();
		ID3D11Device& device = *pRenderer->GetDevice();
		ID3D11DeviceContext& context = *pRenderer->GetDeviceContext();

		// Effekseer�̏��������s��
		renderer = EffekseerRendererDX11::Renderer::Create(&device, &context, EFFECT_DRAW_MAX);
		manager = Effekseer::Manager::Create(EFFECT_DRAW_MAX);
		manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
		manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
		manager->SetRingRenderer(renderer->CreateRingRenderer());
		manager->SetTrackRenderer(renderer->CreateTrackRenderer());
		manager->SetModelRenderer(renderer->CreateModelRenderer());

		// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ�
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
		const DXSimp::Vector3& _pos,
		const DXSimp::Vector3& _scale,
		const DXSimp::Vector3& _eularAngles,
		int _startFrame)
	{
		// Effekseer���ōĐ����s��
		Effekseer::Vector3D pos = { _pos.x, _pos.y, _pos.z };
		Effekseer::Handle handle = manager->Play(_effect, pos, _startFrame);

		// �X�P�[���ݒ�
		manager->SetScale(handle, _scale.x, _scale.y, _scale.z);

		// �p�x
		manager->SetRotation(handle,
			_eularAngles.x * Mathf::degToRad,
			_eularAngles.y * Mathf::degToRad,
			_eularAngles.z * Mathf::degToRad);

		// ���x
		manager->SetSpeed(handle, _playSpeed);

		return  handle;
	}

	Effekseer::Handle DX11EffecseerManager::Play(const CreateVfxInfo& _createVfx, const DXSimp::Vector3& _pos,
		const DXSimp::Vector3& _eularAngles)
	{
		if (!_createVfx.pHitVfx) return -1;

		DXSimp::Vector3 eularAngles = _eularAngles;
		// �����Ȃ�J�����������p�x�����߂�
		if (_createVfx.isLookCamera)
		{
			CalcLookCameraAngles(_pos, eularAngles);
		}

		// �Đ�����
		Effekseer::Handle retHand = Play(_createVfx.pHitVfx->GetEffect(),
			_createVfx.speed,
			_pos,
			_createVfx.scale * DXSimp::Vector3::One,
			eularAngles,
			_createVfx.startFrame);

		// �F��ύX����Ȃ�ύX
		if (_createVfx.isApplyColor)
			ChangeColor(retHand, _createVfx.effectColor);

		return retHand;
	}

	void DX11EffecseerManager::ChangeColor(Effekseer::Handle _efkHandle, const DXSimp::Color& _color)
	{
		manager->SetAllColor(_efkHandle,
			{ static_cast<uint8_t>(_color.R() * 255.0f),
			static_cast<uint8_t>(_color.G() * 255.0f),
			static_cast<uint8_t>(_color.B() * 255.0f),
			static_cast<uint8_t>(_color.A() * 255.0f) });
	}

	void DX11EffecseerManager::DestroyVfx(const Effekseer::Handle _deleteHandle)
	{
		manager->StopEffect(_deleteHandle);
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

		//�]�u�ς݂Ȃ̂œ]�u���Č��ɖ߂�

		// �v���W�F�N�V�����s��
		Effekseer::Matrix44 projMtx;
		CreateEffekseerMtx(pRenderer->GetParameter().GetWVP().projection.Transpose(),
			projMtx);
		renderer->SetProjectionMatrix(projMtx);

		// �J�����s��
		CP_Camera& camera = pInSceneSystem->GetMainCamera();
		renderer->SetCameraMatrix(
			Effekseer::Matrix44().LookAtLH(
				CreateEffekseerVector3(camera.GetEyePos()),
				CreateEffekseerVector3(camera.GetTarget()),
				CreateEffekseerVector3(camera.GetCamUp())
			)
		);
	}

	void DX11EffecseerManager::CalcLookCameraAngles(const DXSimp::Vector3& _efkPos, DXSimp::Vector3& _outAngles)
	{
		DXSimp::Vector3 camPos =
			InSceneSystemManager::GetInstance()->GetMainCamera().GetTransform().GetPosition();

		// �x�N�g������p�x�����߂�
		DXSimp::Vector3 vec = camPos - _efkPos; vec.Normalize();
		_outAngles.y = Quat::RotateToVector(vec).ToEuler().y * Mathf::radToDeg;
	}

	void DX11EffecseerManager::CreateEffekseerMtx(const DXSimp::Matrix& _dxMtx, Effekseer::Matrix44& _outMtx)
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

	Effekseer::Vector3D DX11EffecseerManager::CreateEffekseerVector3(const DXSimp::Vector3& _dxVec3)
	{
		return Effekseer::Vector3D(_dxVec3.x, _dxVec3.y, _dxVec3.z);
	}

	json CreateVfxInfo::Save()
	{
		json vfxData;

		if (pHitVfx)
			vfxData["name"] = pHitVfx->GetAssetName();
		vfxData["applyColor"] = isApplyColor;
		if (isApplyColor) SaveJsonVector4("color", effectColor, vfxData);
		vfxData["lookCam"] = isLookCamera;
		vfxData["scale"] = scale;
		vfxData["speed"] = speed;
		vfxData["startFrame"] = startFrame;

		return vfxData;
	}

	void CreateVfxInfo::Load(const json& _data)
	{
		std::string vfxName;
		if (LoadJsonString("name", vfxName, _data))
		{
			pHitVfx = AssetGetter::GetAsset<VisualEffect>(vfxName);
		}
		if (LoadJsonBoolean("applyColor", isApplyColor, _data))
		{
			LoadJsonColor("color", effectColor, _data);
		}
		LoadJsonBoolean("lookCam", isLookCamera, _data);
		LoadJsonFloat("scale", scale, _data);
		LoadJsonFloat("speed", speed, _data);
		LoadJsonInteger("startFrame", startFrame, _data);
	}

	void CreateVfxInfo::ImGuiDebug()
	{
#ifdef EDIT
		if (!ImGuiMethod::TreeNode("Vfx Info")) return;

		// �q�b�g���G�t�F�N�g
		if (ImGui::Button("Clear"))
			pHitVfx = nullptr;
		std::string vfxName = "Null";
		if (pHitVfx)
			vfxName = pHitVfx->GetAssetName();
		if (AssetGetter::ImGuiGetCombobox<VisualEffect>("HitVfx", vfxName))
		{
			pHitVfx = AssetGetter::GetAsset<VisualEffect>(vfxName);
		}

		// �e�p�����[�^
		ImGui::Checkbox("LookCamera", &isLookCamera);
		ImGui::Checkbox("ApplyColor", &isApplyColor);
		if (isApplyColor)
			ImGui::ColorEdit4("color", &effectColor.x);
		ImGui::DragFloat("Scale", &scale, 0.01f, 0.0f, 1000.0f);
		ImGui::DragFloat("Speed", &speed, 0.01f, 0.0f, 1000.0f);
		ImGui::DragInt("StartFrame", &startFrame, 1.0f, 0, 200);

		ImGui::TreePop();
#endif
	}
}