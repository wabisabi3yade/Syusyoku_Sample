#include "pch.h"
#include "PS_TexColor.h"

namespace HashiTaku
{
	PS_TexColor::PS_TexColor()
	{
		bufferTypes.resize(1);
		bufferTypes[0] = BufferType::Material;
	}
}
