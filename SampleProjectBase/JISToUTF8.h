#pragma once

// ShiftJis→UTF-8にする
#define TO_UTF8(_str) (ShiftJisToUtf8(_str).c_str())

// ImGuiがUTF-8対応なのでShift-JISから変換する関数
std::string ShiftJisToUtf8(std::string const& _jisString);

// stringをwstring
std::wstring StringToWString(std::string const& _string);

std::string WStringToString(std::wstring const& _wstring);