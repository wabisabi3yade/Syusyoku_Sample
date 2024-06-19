#include "pch.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

Transform Geometory::transform = {};
std::unique_ptr<Cube> Geometory::pCube = nullptr;
std::unique_ptr<DebugCube> Geometory::pDebugCube = nullptr;
Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::Init()
{
	pCube = std::make_unique<Cube>();
	pDebugCube = std::make_unique<DebugCube>();
}

void Geometory::Release()
{
	pCube.reset();
	pDebugCube.reset();
}

void Geometory::DrawCube(bool _isWireFrame)
{
	if (_isWireFrame)
		pDebugCube->Draw(transform, color);
	else
		pCube->Draw(transform, color);

	// Œ³‚É–ß‚·
	transform.ResetParameter();
	color = Color(1, 1, 1, 1);
}
