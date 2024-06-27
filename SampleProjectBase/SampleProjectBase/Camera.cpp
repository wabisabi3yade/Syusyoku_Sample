#include "pch.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

Camera::Camera() :focusPos(0, 0, 0), fov(DEFAULT_FOV), nearZ(DEFAULT_NEARZ), farZ(DEFAULT_FARZ),
	isOrthographic(false)
{
	transform.position.z = -10.0f;
	SetPerspective();	// �������e����n�߂�
}

Camera::~Camera()
{
}

void Camera::UpdateViewMatrix()
{
	focusPos = transform.position + transform.Forward();	// �J�����̑O�𒍎��_�ɂ���

	// �r���[�ϊ��s������߂�
	DirectX::SimpleMath::Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		transform.position,		// �J�������W
		focusPos,	// �����_
		Vector3::Up // ��x�N�g��
	);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);

	// �r���[�ϊ��s����Z�b�g
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void Camera::UpdatePerspective(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
	float screenWidth = viewport.Width;
	float screenHeight = viewport.Height;

	// �r���[�ϊ��s����쐬����
	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(
		fov,
		screenWidth / screenHeight,   // �A�X�y�N�g��
		nearZ,
		farZ);
	mat = XMMatrixTranspose(mat);

	// ���e�s��̎Q�Ƃ��擾���A�r���[�ϊ��s���������
	RenderParam& param = renderer.GetParameter();
	param.SetProjection(mat);
}

void Camera::UpdateOrthographic(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
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

void Camera::SetOrthographic()
{
	isOrthographic = true;
	UpdateOrthographic(0);
}

void Camera::SetPerspective()
{
	isOrthographic = false;
	UpdatePerspective(0);
}
