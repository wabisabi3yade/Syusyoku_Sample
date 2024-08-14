#include "pch.h"
#include "CP_Camera.h"

#include "GameObject.h"

using namespace DirectX::SimpleMath;

// �f�t�H���g�l
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

CP_Camera::CP_Camera() : viewPortSlot(0)
{
	name = "Camera";

	// �����l���
	fov = DEFAULT_FOV;
	nearZ = DEFAULT_NEARZ;
	farZ = DEFAULT_FARZ;
	
	SetPerspective();	// �������e����n�߂�
}

void CP_Camera::Init()
{
	GetTransform().position.y = 2.0f;
	GetTransform().position.z = -5.0f;

	//GetTransform().position.y = 200.0f;
	//GetTransform().position.z = 400.0f;
	GetTransform().SetEularAngles(Vector3(10.0f, 0.0f, 0.0f));
}

void CP_Camera::LateUpdate()
{
	UpdateViewMatrix();

	if (isOrthographic)
		UpdateOrthographic();
	else
		UpdatePerspective();
}

void CP_Camera::ImGuiSetting()
{
	ImGui::DragFloat("fov", &fov, 1.0f, 15.0f, 180.0f);
}

void CP_Camera::UpdateViewMatrix()
{
	focusPos = GetTransform().position + GetTransform().Forward();

	if (Vector3::Distance(GetTransform().position, focusPos) < Mathf::smallValue)
	{
		focusPos.z += Mathf::smallValue;
	}

	// �r���[�ϊ��s������߂�
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().position,		// �J�������W
		focusPos,	// �����_
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

void CP_Camera::SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos)
{
	focusPos = _focusPos;
}

void CP_Camera::SetViewportSlot(u_int _slot)
{
	viewPortSlot = _slot;
}

void CP_Camera::UpdatePerspective()
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// �r���[�ϊ��s����쐬����
	Matrix mat = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		screenWidth / screenHeight,   // �A�X�y�N�g��
		nearZ,
		farZ);
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
