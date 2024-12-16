#include "pch.h"
#include "VS_SkinnedAnimation.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

namespace HashiTaku
{
	VS_SkinnedAnimation::VS_SkinnedAnimation()
	{
		bufferTypes.resize(2);
		bufferTypes[0] = BufferType::WVP;
		bufferTypes[1] = BufferType::BoneMatricies;
	}
}