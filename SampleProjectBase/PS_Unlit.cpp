#include "pch.h"
#include "PS_Unlit.h"

constexpr u_int BUFFER_NUM(1);
constexpr u_int LIGHT_SLOT(0);

PS_Unlit::PS_Unlit()
{
	bufferTypes.resize(BUFFER_NUM);
	bufferTypes[LIGHT_SLOT] = BufferType::Light;
}