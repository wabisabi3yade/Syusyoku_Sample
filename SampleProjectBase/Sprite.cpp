#include "pch.h"
#include "Sprite.h"

#include "Material.h"

#include "AssetGetter.h"

using namespace DirectX::SimpleMath;
using namespace HashiTaku;

Sprite::Sprite()
	: pTexture(nullptr), uvStart(Vector2::Zero), uvEnd(Vector2::One), isTextureEnable(false)
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
	uvDivideNum = _other.uvDivideNum;
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
	isTextureEnable = true;
}

Texture* Sprite::GetTexture() const
{
	return pTexture;
}

const PlaneMesh& Sprite::GetSquare() const
{
	return *pSquareMesh.get();
}

bool Sprite::GetIsTexEnable() const
{
	return isTextureEnable;
}

void Sprite::SetUV(const DirectX::SimpleMath::Vector2& _uvStart, const DirectX::SimpleMath::Vector2& _uvEnd)
{
	uvStart = _uvStart;
	uvEnd = _uvEnd;
}

nlohmann::json Sprite::Save()
{
	nlohmann::json data;

	if (pTexture)
		data["texName"] = pTexture->GetAssetName();
	SaveJsonVector2("uvStart", uvStart, data);
	SaveJsonVector2("uvEnd", uvEnd, data);
	SaveJsonVector2("uvDivNum", uvDivideNum, data);

	return data;
}

void Sprite::Load(const nlohmann::json& _data)
{
	pTexture = LoadJsonAsset<Texture>("texName", _data);
	if (pTexture)
		isTextureEnable = true;

	LoadJsonVector2("uvStart", uvStart, _data);
	LoadJsonVector2("uvEnd", uvEnd, _data);
	LoadJsonVector2("uvDivNum", uvDivideNum, _data);
}
