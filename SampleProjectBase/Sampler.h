#pragma once

namespace HashiTaku
{
	// �T���v���[�i�e�N�X�`�����I�u�W�F�N�g�ɓ\��Ƃ��̕�ԕ��@�j
	class Sampler
	{
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	public:
		Sampler() {};
		~Sampler() {};

		bool Init(ID3D11Device& _device);

		ID3D11SamplerState* GetSampler() { return pSampler.Get(); }
	};
}