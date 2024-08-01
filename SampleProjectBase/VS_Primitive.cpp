#include "pch.h"
#include "VS_Primitive.h"

constexpr u_int BUFFER_NUM(2);
constexpr u_int WVP_SLOT(0);
constexpr u_int COLOR_SLOT(1);

VS_Primitive::VS_Primitive()
{
	bufferTypes.resize(BUFFER_NUM);

	bufferTypes[WVP_SLOT] = BufferType::WVP;
	bufferTypes[COLOR_SLOT] = BufferType::Color;
}