#include "pch.h"
#include "VS_Geometory.h"

constexpr u_int BUFFER_NUM(2);
constexpr u_int WVP_SLOT(0);
constexpr u_int COLOR_SLOT(1);

VS_Geometory::VS_Geometory()
{
	bufferTypes.resize(BUFFER_NUM);

	bufferTypes[WVP_SLOT] = BufferType::WVP;
	bufferTypes[COLOR_SLOT] = BufferType::Geometory_Color;
}