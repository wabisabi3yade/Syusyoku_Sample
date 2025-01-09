#pragma once
#include "Shader.h"

namespace HashiTaku
{
	/// @brief ���_�V�F�[�_�[�Ƀo�b�t�@�𑗂�����Ȃǂ���N���X
	class VertexShader : public Shader
	{
		/// @brief ���_�V�F�[�_�[
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVxShader;
	protected:

		/// @brief ���̓��C�A�E�g
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		/// @brief ���̓��C�A�E�g���쐬����
		virtual void MakeInputLayout(const char* _pData, u_int _dataSize);
	public:
		VertexShader();
		~VertexShader();

		/// @brief GPU�ɏ��𑗂�
		void SetGPU() override;

		/// @brief 1���[�v�Ɉ�񂵂��X�V���Ȃ��Ă��悢�o�b�t�@�͂����ōX�V����
		void OnceUpdateBuffer() override {}
	private:

		/// @brief ���_�V�F�[�_�[�쐬
		/// @param _pData �V�F�[�_�[���e
		/// @param _dataSize �f�[�^�̃T�C�Y
		void MakeShader(const char* _pData, u_int _dataSize) override;
	};
}