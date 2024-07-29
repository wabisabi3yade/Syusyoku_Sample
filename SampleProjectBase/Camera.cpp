#include "pch.h"
#include "Camera.h"

#include "GameObject.h"

using namespace DirectX::SimpleMath;

// �f�t�H���g�l
constexpr float DEFAULT_FOV = 45.0f;
constexpr float DEFAULT_NEARZ = 0.1f;
constexpr float DEFAULT_FARZ = 1000.0f;

void Camera::Init()
{
	// �����l���
	focusPos = { 0, 0, 0 };
	fov = DEFAULT_FOV;
	nearZ = DEFAULT_NEARZ;
	farZ = DEFAULT_FARZ;
	
	/*GetTransform().position.y = 2.0f;
	GetTransform().position.z = -5.0f;*/

	GetTransform().position.y = 200.0f;
	GetTransform().position.z = 400.0f;
	GetTransform().rotation.x = 30.0f;
	GetTransform().rotation.y = 180.0f;
	SetPerspective();	// �������e����n�߂�

	gameObject->AddComponent<CameraMove>();
}

void Camera::UpdateViewMatrix()
{
	focusPos = GetTransform().position + GetTransform().Forward() * 1.0f;

	if (Vector3::Distance(GetTransform().position, focusPos) < 0.0001f)
	{
		focusPos.z += 0.001f;
	}

	// �r���[�ϊ��s������߂�
	Matrix viewMatrix = DirectX::XMMatrixLookAtLH
	(
		GetTransform().position,		// �J�������W
		focusPos,	// �����_
		Vector3::Up // ��x�N�g��
	);
	viewMatrix =viewMatrix.Transpose();

	// �r���[�ϊ��s����Z�b�g
	Direct3D11::GetInstance()->GetRenderer()->GetParameter().SetView(viewMatrix);
}

void Camera::SetPerspective(u_int _viewPortSlot)
{
	isOrthographic = false;
	UpdatePerspective(_viewPortSlot);
}

void Camera::SetOrthographic(u_int _viewPortSlot)
{
	isOrthographic = true;
	UpdateOrthographic(_viewPortSlot);
}

void Camera::SetFocusPos(const DirectX::SimpleMath::Vector3& _focusPos)
{
	focusPos = _focusPos;
}

void Camera::UpdatePerspective(u_int _viewPortSlot)
{
	D3D11_Renderer& renderer = *Direct3D11::GetInstance()->GetRenderer();

	// �r���[�|�[�g���擾
	const D3D11_VIEWPORT& viewport = renderer.GetViewPort(_viewPortSlot);
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
