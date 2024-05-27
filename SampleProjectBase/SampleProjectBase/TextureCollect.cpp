#include "pch.h"
#include "TextureCollect.h"
#include "Texture.h"

bool TextureCollect::Load(const char* _pathName, std::string _texName)
{
	// 既にテクスチャがあるなら読み込まない
	if (GetIsImported(_texName))
	{
		return true;
	}
	
	// テクスチャクラスを作成して、ロードを行う
	std::unique_ptr<Texture> tex = std::make_unique<Texture>();
	bool isSucess = tex->Load(_pathName);
	if (!isSucess)
		return false;

	// 成功したら、配列に入れる
	resourceList.emplace(_texName, std::move(tex));

	return true;
}
