#include "pch.h"
#include "CP_PostEffect.h"

namespace HashiTaku
{
	CP_PostEffect::CP_PostEffect(PostEffectType _postType) : 
		postEffectType(_postType)
	{
	}

	CP_PostEffect::PostEffectType CP_PostEffect::GetPostEffectType() const
	{
		return postEffectType;
	}
}

