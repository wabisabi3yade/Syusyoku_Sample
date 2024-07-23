#include "pch.h"
#include "Sprite.h"

#include "Material.h"

using namespace DirectX::SimpleMath;

Sprite::Sprite() : pTexture(nullptr), uvStart(Vector2::Zero), uvEnd(Vector2::One)
{
	pSquareMesh = std::make_unique<PlaneMesh>();
}

void Sprite::Copy(const Sprite& _other)
{
	// アドレスを渡す
	pTexture = _other.pTexture;

	// メッシュを作ってパラメータをコピー
	pSquareMesh = std::make_unique<PlaneMesh>(*_other.pSquareMesh);

	uvStart = _other.uvStart;
	uvEnd = _other.uvEnd;
}

Sprite::Sprite(const Sprite& _other)
{
	if (this == &_other) return;
	Copy(_other);
}

Sprite& Sprite::operator=(const Sprite& _other)
{
	if (this == &_other) return *this;
	Copy(_other);
	return *this;
}

void Sprite::SetTexture(Texture& _tex)
{
	pTexture = &_tex;
}

Texture& Sprite::GetTexture() const
{
	return *pTexture;
}

const PlaneMesh& Sprite::GetSquare() const
{
	return *pSquareMesh.get();
}

void Sprite::SetUV(const DirectX::SimpleMath::Vector2& _uvStart, const DirectX::SimpleMath::Vector2& _uvEnd)
{
	uvStart = _uvStart;
	uvEnd = _uvEnd;
}