#pragma once

namespace HashiTaku
{
	class RenderParam;

	// �`��ɕK�v�ȃ��\�[�X���擾����C���^�[�t�F�[�X
	class IGetRenderResource
	{
	public:
		// �`��̏����擾
		virtual RenderParam& GetParameter() = 0;
		// �f�o�C�X���擾
		virtual ID3D11Device* GetDevice()const = 0;
		// �f�o�C�X�R���e�L�X�g���擾
		virtual ID3D11DeviceContext* GetDeviceContext() = 0;
		//	�X���b�v�`�F�C�����擾
		virtual IDXGISwapChain* GetSwapChain() = 0;
	};
}