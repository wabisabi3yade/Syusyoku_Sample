#pragma once
#include <codecvt>
#include <locale>

// ImGuiがUTF-8対応なのでShift-JISから変換する関数
std::string ShiftJisToUtf8(std::string const& _jisString);

// stringをwstring
std::wstring StringToWString(std::string const& _string);

std::string WStringToString(std::wstring const& _wstring);