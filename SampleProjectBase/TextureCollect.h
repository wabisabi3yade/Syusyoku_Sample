#pragma once
#include "ResourceCollect_Base.h"
#include "Singleton_Base.h"

class Texture;
class TextureCollect :
	public ResourceCollect_Base<Texture>, public Singleton_Base<TextureCollect>
{
	friend class Singleton_Base<TextureCollect>;
	~TextureCollect() {};
public:
	// 読み込んで配列に入れる
	bool Load(const char* _pathName, std::string _texName);
};

