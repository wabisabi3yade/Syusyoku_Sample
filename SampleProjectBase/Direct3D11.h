#pragma once
#include "Singleton_Base.h"
#include <Windows.h>
#include "D3D11_Renderer.h"

// Direct3D11�̋@�\�����N���X
class Direct3D11
	: public Singleton_Base<Direct3D11>
{
	friend class Singleton_Base<Direct3D11>;

	D3D11_Renderer* pRenderer = nullptr;	// �`��@�\�����N���X

	Direct3D11();
	~Direct3D11();

	void Release();	// �������
public:

	void Init(HWND _hwnd);	// ������

	// �`��N���X���擾����
	D3D11_Renderer* GetRenderer() { return pRenderer; }
};

