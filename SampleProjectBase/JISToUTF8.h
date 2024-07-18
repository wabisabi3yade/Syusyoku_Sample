#pragma once

// ShiftJis��UTF-8�ɂ���
#define TO_UTF8(_str) (ShiftJisToUtf8(_str).c_str())

// ImGui��UTF-8�Ή��Ȃ̂�Shift-JIS����ϊ�����֐�
std::string ShiftJisToUtf8(std::string const& _jisString);

// string��wstring
std::wstring StringToWString(std::string const& _string);

std::string WStringToString(std::wstring const& _wstring);