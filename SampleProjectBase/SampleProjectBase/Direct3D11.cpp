#include "Direct3D11.h"

Direct3D11::Direct3D11()
{
}

Direct3D11::~Direct3D11()
{
	Release();
}

void Direct3D11::Release()
{
	CLASS_DELETE(pRenderer);
}

void Direct3D11::Init(HWND _hwnd)
{
	pRenderer = new D3D11_Renderer(_hwnd);
}