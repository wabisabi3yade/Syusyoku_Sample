#pragma once
#include "Singleton_Base.h"
#include <Windows.h>
#include "D3D11_Renderer.h"

namespace HashiTaku
{
	// Direct3D11の機能をもつクラス
	class Direct3D11
		: public Singleton_Base<Direct3D11>
	{
		friend class Singleton_Base<Direct3D11>;

		std::unique_ptr<D3D11_Renderer> pRenderer; // 描画機能を持つクラス

		Direct3D11();
		~Direct3D11();
	public:

		void Init(HWND _hwnd);	// 初期化

		// 描画クラスを取得する
		D3D11_Renderer* GetRenderer() { return pRenderer.get(); }
	};
}
