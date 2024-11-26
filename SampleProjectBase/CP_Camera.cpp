#include "pch.h"
#include "CP_Camera.h"
#include "GameObject.h"
#include "InSceneSystemManager.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

// �f�t�H���g�l
constexpr float DEFAULT_FOV = 45.0f;
constexpr float NEAR_Z = 0.01f;
constexpr float DEFAULT_DISTANCE = 1000.0f;

CP_Camera::CP_Camera() :fov(DEFAULT_FOV), distance(DEFAULT_DISTANCE), viewPortSlot(0)
{	
}

void CP_Camera::Init()
{
	// ���C���J�����ɐݒ肷��
	InSceneSystemManager::GetInstance()->SetCamera(*this);

	SetPerspective();	// �������e����n�߂�
}

void CP_Camera::OnDestroy()
{
	// ���C���J��������O��
	InSceneSystemManager::GetInstance()->DeleteCamera(*this);
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetCamUp() const
{
	return camUp;
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetEyePos() const
{
	return GetTransform().GetPosition();
}

const DirectX::SimpleMath::Vector3& CP_Camera::GetTarget() const
{
	return target;
}

float CP_Camera::GetFov() const
{
	return fov;
}

void CP_Camera::ImGuiDebug()
{
	ImGui::Text("Right");
	ImGuiMethod::Text(GetTransform().Right());
	ImGui::Text("Up");
	ImGuiMethod::Text(GetTransform().Up());
	ImGui::Text("Forward");
	ImGuiMethod::Text(GetTransform().Forward());

	bool isChangeFov = false;
	bool isChangeDis = false;
	isChangeFov = ImGui::DragFloat("fov", &fov, 1.0f, 15.0f, 110.0f);
	isChangeDis = ImGui::DragFloat("distance", &distance, 1.0f, 0.1f, 2000.0f);

	if (isChangeFov || isChangeDis)
	{
		UpdatePerspective();
	}
}

void CP_Camera::UpdateViewMatrix()
{
	// �����_�����߂�
	target = GetTransform().GetPosition() + GetTransform().Forward();

	// �r���[�ϊ��s������߂�
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().GetPosition(),		// �J�������W
		target,	// �����_
		Vector3::Up // ��x�N�g��
	);
	viewMatrix = viewMatrix.Transpose();

	// �r���[�ϊ��s����Z�b�g
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void CP_Camera::SetPerspective()
{
	isOrthographic = false;
	UpdatePerspective();
}

void CP_Camera::SetOrthographic()
{
	isOrthographic = true;
	UpdateOrthographic();
}

void CP_Camera::SetFov(float _setFov)
{
	fov = _setFov;
	UpdatePerspective();
}

void CP_Camera::SetViewportSlot(u_int _slot)
{
	viewPortSlot = _slot;
}

nlohmann::json CP_Camera::Save()
{
	auto data = Component::Save();

	data["fov"] = fov;
	data["distance"] = distance;
	data["viewSlot"] = viewPortSlot;

	return data;
}

void CP_Camera::Load(const nlohmann::json& _data)
{
	Component::Load(_data);

	LoadJsonFloat("fov", fov, _data);
	LoadJsonFloat("distance", distance, _data);
	LoadJsonUnsigned("viewSlot", viewPortSlot, _data);
}

void CP_Camera::UpdateProjection()
{
	if (isOrthographic)
		UpdateOrthographic();
	else
		UpdatePerspective();
}

void CP_Camera::UpdatePerspective()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	float nearZ = NEAR_Z;
	float farZ = nearZ + distance;

	// �r���[�ϊ��s����쐬����
	Matrix mat = DirectX::XMMatrixPerspectiveFovLH(
		fov * Mathf::degToRad,
		screenWidth / screenHeight,   // �A�X�y�N�g��
		nearZ,	// �`��ŋ�
		farZ);	// �`��ŉ�

	mat = mat.Transpose();

	// ���e�s��̎Q�Ƃ��擾���A�r���[�ϊ��s���������
	RenderParam& param = renderer.GetParameter();
	param.SetProjection(mat);
}

void CP_Camera::UpdateOrthographic()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// �����e�s����쐬����
	Matrix mat = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f,	// ����
		screenWidth, 		// �E��
		screenHeight,		// ����
		0.0f,	// �E��
		0.0f,
		1.0f);

	mat = mat.Transpose();

	// ���e�s��̎Q�Ƃ��擾���A�r���[�ϊ��s���������
	RenderParam& param = renderer.GetParameter();
	param.SetProjection(mat);
}
