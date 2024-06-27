#pragma once
// ImGui‚ªUTF-8‘Î‰‚È‚Ì‚ÅShift-JIS‚©‚ç•ÏŠ·‚·‚éŠÖ”
std::string ShiftJisToUtf8(std::string const& _jisString);

// string‚ğwstring
std::wstring StringToWString(std::string const& _string);

std::string WStringToString(std::wstring const& _wstring);