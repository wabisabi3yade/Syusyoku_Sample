#pragma once

// サンプラー（テクスチャをオブジェクトに貼るときの補間方法）
class Sampler
{
	ID3D11SamplerState* pSampler;

public:
	Sampler();
	~Sampler();

	bool Init(ID3D11Device& _device);

	ID3D11SamplerState* GetSampler() { return pSampler; }
};

