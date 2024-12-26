#pragma once

namespace HashiTaku
{
	// �`��ɕK�v�ȃp�����[�^�N���X
	class RenderParam
	{
	public:

		/// @brief ���[���h�E�r���[�E�v���W�F�N�V����	
		struct WVP
		{
			DXSimp::Matrix world;	// ���[���h�ϊ��s��
			DXSimp::Matrix view;	// �r���[�ϊ�
			DXSimp::Matrix projection;	// �v���W�F�N�V����
		};

	private:

		/// @brief ���[���h�ϊ��s��
		WVP wvp;
	public:
		RenderParam() {}
		~RenderParam() {}

		WVP& GetWVP();

		/// @brief WVP�ϊ��s����쐬���ēn��
		/// @param _transform ���W
		/// @param _scaleTimes �X�P�[���{��
		/// @return wvp�s��
		WVP& GetWVP(const Transform& _transform);

		/// @brief WVP�ϊ��s����쐬���ēn��
		/// @param _position ���W
		/// @param _scale �傫��
		/// @param _rotation ��]��
		/// @return wvp�s��
		WVP& GetWVP(const DXSimp::Vector3& _position, const DXSimp::Vector3& _scale, const DXSimp::Quaternion& _rotation);

		/// @brief ���݂̓��e�s����擾
		/// @return ���e�s��
		const DXSimp::Matrix& GetProjectionMatrix() const;

		/// @brief �r���[�ϊ��s����Z�b�g
		/// @param _view �r���[�ϊ��s��
		void SetView(const DXSimp::Matrix& _view) { wvp.view = _view; }

		/// @brief �v���W�F�N�V�����ϊ��s����Z�b�g
		/// @param _proj �v���W�F�N�V�����ϊ��s��
		void SetProjection(const DXSimp::Matrix& _proj) { wvp.projection = _proj; }
	};
}