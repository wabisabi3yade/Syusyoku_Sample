#include "Direct3D11.h"

namespace HashiTaku
{
	Direct3D11::Direct3D11()
	{
	}

	Direct3D11::~Direct3D11()
	{
	}

	void Direct3D11::Init(HWND _hwnd)
	{
		pRenderer = std::make_unique<D3D11_Renderer>();
		if (!pRenderer->Init(_hwnd))
		{
			assert(!"D3D11描画クラス初期化でエラー");
		}			
	}
}