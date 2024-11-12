#include "pch.h"
#include "VS_Object.h"

VS_Object::VS_Object()
{
	bufferTypes.resize(1);
	bufferTypes[0] = BufferType::WVP;
}
