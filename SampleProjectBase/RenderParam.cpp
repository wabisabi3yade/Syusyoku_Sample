#include "pch.h"

#include "D3D11_Renderer.h"
#include "RenderParam.h"

ID3D11InputLayout& RenderParam::GetInputLayout()
{
	return *pInputLayout.Get();
}

const RenderParam::WVP& RenderParam::GetWVP()
{
	return wvp;
}

