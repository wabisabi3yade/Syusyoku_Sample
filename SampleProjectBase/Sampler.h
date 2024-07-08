#pragma once

// �T���v���[�i�e�N�X�`�����I�u�W�F�N�g�ɓ\��Ƃ��̕�ԕ��@�j
class Sampler
{
	std::unique_ptr<ID3D11SamplerState> pSampler;
public:
	Sampler(){};
	~Sampler(){};

	bool Init(ID3D11Device& _device);

	ID3D11SamplerState* GetSampler() { return pSampler.get(); }
};

