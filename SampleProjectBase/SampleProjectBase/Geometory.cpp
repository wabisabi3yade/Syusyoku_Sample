#include "pch.h"
#include "Geometory.h"

using namespace DirectX::SimpleMath;

Transform Geometory::transform = {};
std::unique_ptr<Cube> Geometory::pCube = nullptr;
std::unique_ptr<DebugCube> Geometory::pDebugCube = nullptr;
std::unique_ptr<Sphere> Geometory::pSphere = nullptr;
Color Geometory::color = Color(1.0f, 1.0f, 1.0f, 1.0f);

void Geometory::Init()
{
	pCube = std::make_unique<Cube>();
	pDebugCube = std::make_unique<DebugCube>();
	pSphere = std::make_unique<Sphere>();
}

void Geometory::Release()
{
#ifdef _DEBUG
	pCube.reset();
	pDebugCube.reset();
	pSphere.reset();
#endif // _DEBUG
}

void Geometory::DrawCube(bool _isWireFrame)
{
	if (_isWireFrame)
		pDebugCube->Draw(transform, color);
	else
		pCube->Draw(transform, color);

	// 元に戻す
	transform.ResetParameter();
	color = Color(1, 1, 1, 1);
}

void Geometory::DrawSphere(bool _isWireFrame)
{

	pSphere->DebugDraw(transform, color, _isWireFrame);

	// 元に戻す
	transform.ResetParameter();
	color = Color(1, 1, 1, 1);
}
