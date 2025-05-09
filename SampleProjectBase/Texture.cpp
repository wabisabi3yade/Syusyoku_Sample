#include "pch.h"
#include "Texture.h"

namespace HashiTaku
{
	void Texture::SetSRV(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _pSRV)
	{
		pSRV = std::move(pSRV);
	}

	void Texture::SetWidth(u_int _width)
	{
		width = _width;
	}

	void Texture::SetHeight(u_int _height)
	{
		height = _height;
	}

	bool Texture::GetIsSetSRV() const
	{
		return pSRV != nullptr;
	}
}