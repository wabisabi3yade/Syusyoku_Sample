#pragma once

// �`��ɕK�v�ȃp�����[�^�N���X
class RenderParam
{
public:

	/// @brief ���[���h�E�r���[�E�v���W�F�N�V����	
	struct WVP	
	{
		DirectX::SimpleMath::Matrix world;	// ���[���h�ϊ��s��
		DirectX::SimpleMath::Matrix view;	// �r���[�ϊ�
		DirectX::SimpleMath::Matrix projection;	// �v���W�F�N�V����
	};

private:

	/// @brief ���[���h�ϊ��s��
	WVP wvp;	
public:
	RenderParam() {}
	~RenderParam() {}

	/// @brief WVP�ϊ��s����쐬���ēn��
	/// @param _transform ���W
	/// @return wvp�s��
	const WVP& GetWVP(const Transform& _transform);

	/// @brief �r���[�ϊ��s����Z�b�g
	/// @param _view �r���[�ϊ��s��
	void SetView(const DirectX::SimpleMath::Matrix& _view) { wvp.view = _view; }

	/// @brief �v���W�F�N�V�����ϊ��s����Z�b�g
	/// @param _proj �v���W�F�N�V�����ϊ��s��
	void SetProjection(const DirectX::SimpleMath::Matrix& _proj) { wvp.projection = _proj; }
};

