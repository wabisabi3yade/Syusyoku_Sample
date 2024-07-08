#include "pch.h"
#include<filesystem>

#include "Texture.h"
#include "DirectXTex/TextureLoad.h"

void Texture::Release()
{
	SAFE_RELEASE(pSRV);
}

Texture::Texture() : width(0), height(0), pSRV(nullptr)
{
}

Texture::~Texture()
{
	Release();
}
