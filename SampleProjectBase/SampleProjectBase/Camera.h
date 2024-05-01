#pragma once

// �Q�[�����J�����N���X
class Camera
{
	// �J�����̍��W
	Float3 eyePos{ 0,0,0 };
	// �J�����̒����_
	Float3 focusPos{ 0,0,0 };
	// �J�����̏�x�N�g��
	Float3 camUp{ 0,1.f,0 };

public:
	Camera();
	~Camera();

	void Update();

	DirectX::XMMATRIX GetViewMatrix()const;
};

