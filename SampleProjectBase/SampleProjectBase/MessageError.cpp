#include "pch.h"
#include "MessageError.h"

void MessageError(LPCSTR _errorText)
{
#ifdef _DEBUG

	MessageBoxA(NULL, _errorText, "�G���[", MB_OK | MB_ICONERROR);

#endif // _DEBUG
}
