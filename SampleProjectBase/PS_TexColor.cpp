#include "pch.h"
#include "PS_TexColor.h"

namespace HashiTaku
{
	PS_TexColor::PS_TexColor()
	{
		bufferTypes.resize(2);
		bufferTypes[0] = BufferType::Material;
	}
}
